#include "include/onvifApi/onvifApi.h"
#include "include/httplib/httplib.h"
#include <jsoncpp/json/json.h>
#include "libOnvif/wsdd.nsmap"
#include <unistd.h>

int main(int argc, char** argv)
{
    std::string ip = "192.168.1.108";
    int port = 80;
    std::string username = "admin";
    std::string password = "abcd1234";
    bool useHttps = false;
    OnvifApi* onvifApi = new OnvifApi(ip, port, username, password, useHttps);
    std::cout << onvifApi->getEndpointDevice() << std::endl;
    std::cout << onvifApi->getEndpointMedia() << std::endl;

    _tds_onvif_GetDeviceInformationResponse tds_onvif_GetDeviceInformationResponse;
    if(onvifApi->device_GetDeviceInformation(tds_onvif_GetDeviceInformationResponse)){
        std::cout << tds_onvif_GetDeviceInformationResponse.Manufacturer << std::endl;
        std::cout << tds_onvif_GetDeviceInformationResponse.Model << std::endl;
        std::cout << tds_onvif_GetDeviceInformationResponse.FirmwareVersion << std::endl;
        std::cout << tds_onvif_GetDeviceInformationResponse.SerialNumber << std::endl;
        std::cout << tds_onvif_GetDeviceInformationResponse.HardwareId << std::endl;
    }

    std::vector<_ns1_onvif_Profile> ns1_onvif_Profiles;
    if(onvifApi->media_GetProfiles(ns1_onvif_Profiles)){
        for(long unsigned int i = 0; i < ns1_onvif_Profiles.size(); i++){
            std::cout << __FILE__ << " - " << __func__ << " - " << __LINE__ << std::endl;
            std::cout << "Profile token: " << ns1_onvif_Profiles[i].token << std::endl;

            _ns1_onvif_GetStreamUri ns1_onvif_GetStreamUri;
            ns1_onvif_GetStreamUri.ProfileToken = ns1_onvif_Profiles[i].token;
            ns1_onvif_GetStreamUri.Protocol = "RtspUnicast";
            _ns1_onvif_GetStreamUriResponse ns1_onvif_GetStreamUriResponse;
            if(onvifApi->media_GetStreamUri(ns1_onvif_GetStreamUri, ns1_onvif_GetStreamUriResponse)){
                std::cout << "Uri: " << ns1_onvif_GetStreamUriResponse.Uri << std::endl;
                if(ns1_onvif_GetStreamUriResponse.Uri == "rtsp://192.168.1.108:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif"){
                    _ns1_onvif_GetVideoEncoderConfiguration ns1_onvif_GetVideoEncoderConfigurations;
                    std::vector<_ns1_onvif_VideoEncoderConfiguration> ns1_onvif_VideoEncoderConfigurations;
                    if(onvifApi->media_GetVideoEncoderConfigurations(ns1_onvif_GetVideoEncoderConfigurations , ns1_onvif_VideoEncoderConfigurations)){
                        for(long unsigned int i = 0; i < ns1_onvif_VideoEncoderConfigurations.size(); i++){
                            std::cout << "Encoding: " << ns1_onvif_VideoEncoderConfigurations[i].Encoding << std::endl;
                            if(ns1_onvif_VideoEncoderConfigurations[i].Encoding != "H264"){
                                ns1_onvif_VideoEncoderConfigurations[i].Encoding = "H264";
                                if(onvifApi->media_SetVideoEncoderConfiguration(ns1_onvif_VideoEncoderConfigurations[i])){
                                    std::cout << "Set VideoEncoder OK" << std::endl;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }

    // std::vector<_ns1_onvif_VideoEncoderConfiguration> ns1_onvif_VideoEncoderConfigurations;
    // if(onvifApi->media_GetVideoEncoderConfigurations(ns1_onvif_VideoEncoderConfigurations)){
    //     for(long unsigned int i = 0; i < ns1_onvif_VideoEncoderConfigurations.size(); i++){
    //         std::cout << "token: " << ns1_onvif_VideoEncoderConfigurations[i].token << std::endl;
    //         std::cout << "Encoding: " << ns1_onvif_VideoEncoderConfigurations[i].Encoding << std::endl;
    //         std::cout << "Profile: " << ns1_onvif_VideoEncoderConfigurations[i].Profile << std::endl;
    //     }
    // }

    // ns1_onvif_SetVideoEncoderConfiguration.token = "default_1_h264";
    // ns1_onvif_SetVideoEncoderConfiguration.Encoding = _tt_onvif_VideoResolution_JPEG;
    // ns1_onvif_SetVideoEncoderConfiguration.Resolution.Width = 800;
    // ns1_onvif_SetVideoEncoderConfiguration.Resolution.Height = 450;
    // ns1_onvif_SetVideoEncoderConfiguration.Quality = 80.0;
    // ns1_onvif_SetVideoEncoderConfiguration.RateControl.FrameRateLimit = 20;
    // ns1_onvif_SetVideoEncoderConfiguration.RateControl.EncodingInterval = 1;
    // ns1_onvif_SetVideoEncoderConfiguration.RateControl.BitrateLimit = 100000;
    // if(onvifApi->media_SetVideoEncoderConfiguration(ns1_onvif_SetVideoEncoderConfiguration)){
    //     std::cout << "Set VideoEncoder OK" << std::endl;
    // }

}