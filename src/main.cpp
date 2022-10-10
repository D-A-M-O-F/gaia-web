#include <drogon/drogon.h>

int main() {
    drogon::app().registerHandler(
        "/upload_endpoint",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
            drogon::MultiPartParser fileUpload;
            if (fileUpload.parse(req) != 0 || fileUpload.getFiles().size() != 1)
            {
                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setBody("Must only be one file");
                resp->setStatusCode(drogon::k403Forbidden);
                callback(resp);
                return;
            }

            auto &file = fileUpload.getFiles()[0];
            auto md5 = file.getMd5();
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setBody(
                "The server has calculated the file's MD5 hash to be " + md5);
            file.save();
            LOG_INFO << "The uploaded file has been saved to the ./uploads directory";
            callback(resp);
        },
        {drogon::Post});

    //Load config file
    drogon::app().loadConfigFile("config.json");
    //Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
