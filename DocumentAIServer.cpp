//
// Created by yangjun on 2022/9/27.
//

#include "HTTPAPI/DocumentAIServer.h"

#if KDAN_WITH_TOOLS_SERVER

#include "LicenseGenerator.h"
#include "ModelEncode.h"
#include "LicenseVerify.h"
#include "ModelDecode.h"

#endif

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "DateTimeUtils.h"
#include "NumberUtils.h"
#include "FileUtils.h"
#include "StringUtils.h"

namespace DocumentAI {
namespace HTTPAPI {

using namespace rapidjson;
using namespace httplib;
using namespace DocumentAI::Component::License;


void responseHandle(int code, std::string &data, std::string &msg, Response &res) {
    Document doc;
    doc.SetObject();
    Document::AllocatorType &allocator = doc.GetAllocator();
    doc.AddMember("code",code,allocator);
    doc.AddMember("data",data,allocator);
    doc.AddMember("msg",msg,allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string response = std::string(buffer.GetString());
    res.set_content(response, "application/json");
}

DocumentAIServer::DocumentAIServer(int port) {
    port_ = port;
    svr.set_keep_alive_max_count(100); // Default is 5
    svr.set_keep_alive_timeout(10);  // Default is 5
//    svr.set_default_headers()
    svr.set_read_timeout(10, 0); // 5 seconds
    svr.set_write_timeout(10, 0); // 5 seconds

//    svr.Options("/(.*)",
//                [&](const Request & /*req*/, Response &res) {
//                    res.set_header("Access-Control-Allow-Methods", " POST, GET, OPTIONS");
//                    res.set_header("Content-Type", "text/html; charset=utf-8");
//                    res.set_header("Access-Control-Allow-Headers", "X-Requested-With, Content-Type, Accept");
//                    res.set_header("Access-Control-Allow-Origin", "*");
//                    res.set_header("Connection", "close");
//                });

    auto ret = svr.set_mount_point("/", "./www");
    auto ret1 = svr.set_mount_point("/", "./license");
    auto ret2 = svr.set_mount_point("/", "./");
    auto ret3 = svr.set_mount_point("/", "./models");
    if (!ret || !ret1 || !ret2 || !ret3) {
        // The specified base directory doesn't exist...
        DAINFO << "The specified base directory doesn't exist..." << std::endl;
    }
}

DocumentAIServer::~DocumentAIServer() {
    svr.stop();
}

void DocumentAIServer::Run() {

#ifdef KDAN_WITH_TOOLS_SERVER

    svr.Get("/", [&](const auto &req, auto &res) {
        res.set_redirect("index.html");
    });

    svr.Post("/license_generate", [&](const auto &req, auto &res) {
//        res.set_header("Access-Control-Allow-Origin", "*");
        std::string u_id;
        std::string app_id;
        std::string expire_date;
        int platform = 0;
        std::string password;
        std::string ability = "all";
        std::string creator = "DA_PIX_FMT_UNKNOWN";
        std::string private_key ;

        int response_code = -1;
        std::string response_data;
        std::string response_msg;

        if (req.is_multipart_form_data()) {
            DAINFO << "is_multipart_form_data" << std::endl;
            if (req.has_file("password")) {
                auto req_password = req.get_file_value("password");
                password = req_password.content;
                if (password.compare("wangshuai0327") == 0) {
                    creator = "wangshuai";
                }else if (password.compare("yangjun0116") == 0){
                    creator = "yangjun";
                }else if (password.compare("wangchaoniubi") == 0){
                    creator = "wangchao";
                }


                if (creator.compare("DA_PIX_FMT_UNKNOWN") != 0){
                    auto req_u_id = req.get_file_value("user_id");
                    u_id = req_u_id.content;

                    auto req_app_id = req.get_file_value("app_id");
                    app_id = req_app_id.content;

                    auto req_expire_date = req.get_file_value("expire_date");
                    expire_date = req_expire_date.content;

                    auto req_platform = req.get_file_value("platform");
                    std::string platform_str = req_platform.content;

                    platform = stringToNum<int>(platform_str);

                    auto req_private_key = req.get_file_value("private_key");

                    private_key = req_private_key.content;
                    DAINFO << "private_key: " << private_key << std::endl;

                    auto license_ = LicenseGenerator(u_id, app_id, expire_date, ability, creator, platform);
                    std::string license_path;
                    bool g_r = license_.generatorLicense(private_key,license_path, false);

                    if (g_r){

                        std::string license_str = ReadFileToString(license_path);

                        Document doc;
                        doc.SetObject();
                        Document::AllocatorType &allocator = doc.GetAllocator();

                        doc.AddMember("license_str",license_str,allocator);
                        doc.AddMember("license_file",license_path,allocator);
                        StringBuffer buffer;
                        Writer<StringBuffer> writer(buffer);
                        doc.Accept(writer);
                        response_data = std::string(buffer.GetString());
                        response_code = 0;

//        license_.generatorLicense()
                        responseHandle(response_code, response_data, response_msg, res);
                    }else {
                        response_code = -1003;
                        response_msg = "private key file is invalid!";
                        responseHandle(response_code, response_data, response_msg, res);
                    }
                }else {
                    response_code = -1001;
                    response_data = password;
                    response_msg = "password is incorrect!";
                    responseHandle(response_code, response_data, response_msg, res);
                }
            }else {
                response_code = -1002;
                response_msg = "Unauthorized access";

                responseHandle(response_code, response_data, response_msg, res);
            }
        }else {
            response_code = -1004;
            response_msg = "You should use Content-Type: multipart/form-data;";
            responseHandle(response_code, response_data, response_msg, res);
        }
//
//


    });

    svr.Get("/license_verify", [&](const auto &req, auto &res) {


    });


    svr.Post("/model_generate", [&](const auto &req, auto &res) {
        int response_code = -1;
        std::string response_data;
        std::string response_msg;
        if (req.is_multipart_form_data()) {
            std::string public_key;
            std::string models;
            std::string license;
            std::string expire_date;

            if (req.has_file("public_key") && req.has_file("models")&& req.has_file("license") && req.has_file("expire_date") )
            {
                auto req_public_key = req.get_file_value("public_key");
                auto req_models = req.get_file_value("models");
                auto req_license = req.get_file_value("license");
                auto req_expire_date = req.get_file_value("expire_date");
                public_key = req_public_key.content;
                models = req_models.content;
                license = req_license.content;
                expire_date = req_expire_date.content;

                LicenseVerify licenseVerify(license, false);
                da_result_t  ret = licenseVerify.verifyLicense();

                if (ret != E_DA_SUCCESS){
                    response_code = ret-1000;
                    response_msg = "license is invalid!";
                    responseHandle(response_code, response_data, response_msg, res);
                    return ;
                }
                ret = licenseVerify.verifyKey(public_key);
                if (ret != E_DA_SUCCESS){
                    response_code = ret-1000;
                    response_msg = "license and public key mismatch!";
                    responseHandle(response_code, response_data, response_msg, res);
                    return ;
                }

                int platform = licenseVerify.getPlatform();
                int infer_type = 0;
                if (platform & Component::PLATFORM_ANDROID || platform & Component::PLATFORM_IOS
                    || platform & Component::PLATFORM_MAC_M1){
                    infer_type = infer_type | Component::INFER_TYPE_LITE;
                }
                if (platform & Component::PLATFORM_LINUX_X64 || platform & Component::PLATFORM_WINDOWS
                    || platform & Component::PLATFORM_MAC_X64){
                    infer_type = infer_type | Component::INFER_TYPE_INFER;
                }

                if (infer_type == (Component::INFER_TYPE_LITE | Component::INFER_TYPE_INFER)){
                    infer_type = Component::INFER_TYPE_ALL;
                }

                std::vector<std::string> model_array = Stringsplit(models,",");

                Component::ModelCodec::ModelEncoder modelEncoder(license,public_key);
                modelEncoder.setRootPath("/Users/yangjun/jerome/work/kmpdfkit_ocr_models");
                std::string create_model_dir =  "models/"+licenseVerify.getAppID()+ "_" +std::to_string(platform);
                std::string rm_cmd = "rm -rf " + create_model_dir;
                system(rm_cmd.c_str());
                std::string create_dir_cmd = "mkdir -p "+ create_model_dir;
                system(create_dir_cmd.c_str());
                da_result_t encode_ret ;
                std::string encode_ret_model;
                for (auto iter : model_array)
                {
                    if (iter.compare("ocr_rec_eng") == 0){
                        encode_ret = modelEncoder.encode("/config/ocr/rec/rec_eng.txt",expire_date,infer_type,create_model_dir);
                    }else if (iter.compare("ocr_rec_chinese") == 0){
                        encode_ret = modelEncoder.encode("/config/ocr/rec/rec_chinese.txt",expire_date,infer_type,create_model_dir);

                    }else if (iter.compare("ocr_rec_chinese_cht") == 0){
                        encode_ret = modelEncoder.encode("/config/ocr/rec/rec_chinese_cht.txt",expire_date,infer_type,create_model_dir);
                    }else if (iter.compare("ocr_rec_japan") == 0){
                        encode_ret = modelEncoder.encode("/config/ocr/rec/rec_japan.txt",expire_date,infer_type,create_model_dir);

                    }else if (iter.compare("ocr_rec_korean") == 0){
                        encode_ret = modelEncoder.encode("/config/ocr/rec/rec_korean.txt",expire_date,infer_type,create_model_dir);

                    }else if (iter.compare("ocr_det_eng") == 0){
                        encode_ret = modelEncoder.encode("/config/ocr/det/det_eng.txt",expire_date,infer_type,create_model_dir);

                    }else if (iter.compare("ocr_det_chinese") == 0){
                        encode_ret = modelEncoder.encode("/config/ocr/det/det_chinese.txt",expire_date,infer_type,create_model_dir);

                    }else if (iter.compare("ocr_cls") == 0){
                        encode_ret = modelEncoder.encode("/config/ocr/cls/cls.txt",expire_date,infer_type,create_model_dir);
                    }else if (iter.compare("table_rec") == 0){
                        encode_ret = modelEncoder.encode("/config/table_rec/det_chinese.txt",expire_date,infer_type,create_model_dir);
                    }else if (iter.compare("detection_layout_parser") == 0){
                        encode_ret = modelEncoder.encode("/config/detection/layout_parser/DET_TA_F_202211161623.txt",expire_date,infer_type,create_model_dir);
                    } else if (iter.compare("image_magic_color") == 0){
                        encode_ret = modelEncoder.encode("/config/image_enhance/image_enhance_magic_color.txt",expire_date,infer_type,create_model_dir);
                    }
                    if (encode_ret != E_DA_SUCCESS){
                        encode_ret_model = iter;
                        break;
                    }
                }

                if (encode_ret != E_DA_SUCCESS){
                    response_code = -1008;
                    response_msg = "Model Encode Failed : " + encode_ret_model;
                    responseHandle(response_code, response_data, response_msg, res);
                    return ;
                }
                std::string zip_name;
                if (licenseVerify.getFlag().empty()){
                    zip_name = licenseVerify.getAppID()+ "_" +std::to_string(platform) +".zip";
                }else {
                    zip_name = licenseVerify.getFlag()+ ".zip";
                }
                std::string rm_cmd_1 = "rm -rf models/" + zip_name;
                system(rm_cmd_1.c_str());
                std::string cmd = "zip -q -r models/" + zip_name +" " + create_model_dir;
                system(cmd.c_str());
//                modelEncoder.encode()
//                ret = modelEncoder.encode()
                DAINFO << "req_models:" << models << std::endl;

                Document doc;
                doc.SetObject();
                Document::AllocatorType &allocator = doc.GetAllocator();

//                doc.AddMember("model",license_str,allocator);
                doc.AddMember("model_file",zip_name,allocator);
                StringBuffer buffer;
                Writer<StringBuffer> writer(buffer);
                doc.Accept(writer);
                response_data = std::string(buffer.GetString());
                response_code = 0;
                response_msg = "";
                responseHandle(response_code, response_data, response_msg, res);

            }else {
                response_code = -1006;
                response_msg = "missing parameter.";
                responseHandle(response_code, response_data, response_msg, res);
            }





        }else {
            response_code = -1004;
            response_msg = "You should use Content-Type: multipart/form-data;";
            responseHandle(response_code, response_data, response_msg, res);
        }
    });
#endif
    DAINFO << "hello" << std::endl;
    svr.listen("0.0.0.0", port_);

}


} //namespace HTTPAPI
} //namespace DocumentAI