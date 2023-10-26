#ifndef OnvifApi_H
#define OnvifApi_H

#include "../../libOnvif/soapDeviceBindingProxy.h"
#include "../../libOnvif/soapAdvancedSecurityServiceBindingProxy.h"
// #include "../../libOnvif/soapCredentialBindingProxy.h"
// #include "../../libOnvif/soapDeviceIOBindingProxy.h"
// #include "../../libOnvif/soapReceiverBindingProxy.h"
// #include "../../libOnvif/soapRecordingBindingProxy.h"
// #include "../../libOnvif/soapReplayBindingProxy.h"
// #include "../../libOnvif/soapRuleEngineBindingProxy.h"
// #include "../../libOnvif/soapScheduleBindingProxy.h"
#include "../../libOnvif/soapMedia2BindingProxy.h"
#include "../../libOnvif/soapImagingBindingProxy.h"
#include "../../libOnvif/soapPTZBindingProxy.h"
#include "../../libOnvif/soapPullPointSubscriptionBindingProxy.h"
#include "../../libOnvif/soapRemoteDiscoveryBindingProxy.h" 
#include "../../libOnvif/wsddapi.h"
#include "../../libOnvif/wsseapi.h"
#include "onvifCustomStruct.h"

struct ClientInfo
{
    std::string ip;
    int port;
    std::string username;
    std::string password;
    bool useHttps = false;
    bool isDigestLogin = false;
};


class OnvifApi
{
public:
    /* data */
    struct soap *soap;
    ClientInfo clientInfo;

    OnvifApi(std::string ip, int port, std::string username, std::string password, bool useHttps);
    OnvifApi(struct soap *soap);
    // void init();
    //add username and password before login
    void set_credentials(std::string username, std::string password, bool isDigestLogin);
    //report error
    void report_error();
    void changeClientInfo(std::string ip, int port, std::string username, std::string password, bool useHttps);
    std::string getEndpointDevice();
    std::string getEndpointMedia();
    std::string getEndpointImaging();
    std::string getEndpointPtz();
    std::string getEndpointEvent();
    
    // For Onvif Device Service
    bool device_AddScopes(std::vector<std::string> ScopeItem);
    bool device_CreateUsers(_tds__CreateUsers *tds__CreateUsers, _tds__CreateUsersResponse &tds__CreateUsersResponse);
    bool device_DeleteUsers(ClientInfo clientInfo, _tds__DeleteUsers *tds__DeleteUsers, _tds__DeleteUsersResponse &tds__DeleteUsersResponse);
    bool device_GetCapabilities(ClientInfo clientInfo, _tds__GetCapabilities *tds__GetCapabilities, _tds__GetCapabilitiesResponse &tds__GetCapabilitiesResponse);
    bool device_GetDeviceInformation(_tds_onvif_GetDeviceInformationResponse &tds_onvif_GetDeviceInformationResponse);
    bool device_GetDiscoveryMode(_tds_onvif_DiscoveryMode &tds_onvif_DiscoveryMode);
    bool device_GetDNS(_tds_onvif_DNSInformation& tds_onvif_DNSInformation);
    bool device_GetHostname(_tds_onvif_HostnameInformation& tds_onvif_HostnameInformation);
    bool device_GetNetworkDefaultGateway(_tds_onvif_NetworkGateway& tds_onvif_NetworkGateway);
    bool device_GetNetworkInterfaces(std::vector<_tds_onvif_GetNetworkInterfacesResponse> &tds_onvif_GetNetworkInterfacesResponse);
    bool device_GetNetworkProtocols(std::vector<_tds_onvif_NetworkProtocol>& tds_onvif_NetworkProtocol);
    bool device_GetNTP(ClientInfo clientInfo, _tds__GetNTP *tds__GetNTP, _tds__GetNTPResponse &tds__GetNTPResponse);
    bool device_GetScopes(std::vector<_tds_onvif_Scope>& tds_onvif_ScopeResponse);
    bool device_GetSystemDateAndTime(_tds_onvif_SystemDateTime& tds_onvif_SystemDateTime);
    bool device_GetSystemLog(_tds_onvif_LogType tds_onvif_LogType, std::string& String);
    bool device_GetUsers(ClientInfo clientInfo, _tds__GetUsers *tds__GetUsers, _tds__GetUsersResponse &tds__GetUsersResponse);
    bool device_GetWsdlUrl(ClientInfo clientInfo, _tds__GetWsdlUrl *tds__GetWsdlUrl, _tds__GetWsdlUrlResponse &tds__GetWsdlUrlResponse);
    bool device_RemoveScopes(std::vector<std::string> ScopeItem, std::vector<std::string> &ScopeItemResponse);
    bool device_SetDiscoveryMode(_tds_onvif_DiscoveryMode tds_onvif_DiscoveryMode);
    bool device_SetHostname(std::string Name);
    bool device_SetIPAddressFilter(ClientInfo clientInfo, _tds__SetIPAddressFilter *tds__SetIPAddressFilter, _tds__SetIPAddressFilterResponse &tds__SetIPAddressFilterResponse);
    bool device_SetNetworkDefaultGateway(_tds_onvif_NetworkGateway tds_onvif_NetworkGateway);
    bool device_SetNetworkInterfaces(_tds_onvif_SetNetworkInterfaces tds_onvif_SetNetworkInterfaces, bool &RebootNeeded);
    bool device_SetNetworkProtocols(std::vector<_tds_onvif_NetworkProtocol> tds_onvif_NetworkProtocol);
    bool device_SetNTP(ClientInfo clientInfo, _tds__SetNTP *tds__SetNTP, _tds__SetNTPResponse &tds__SetNTPResponse);
    bool device_SetScopes(std::vector<std::string> Scopes);
    bool device_SetSystemDateAndTime(_tds_onvif_SetSystemDateAndTime tds_onvif_SetSystemDateAndTime);
    bool device_SetSystemFactoryDefault(_tds_onvif_FactoryDefault tds_onvif_FactoryDefault);
    bool device_SetUser(ClientInfo clientInfo, _tds__SetUser *tds__SetUser, _tds__SetUserResponse &tds__SetUserResponse);
    bool device_SystemReboot(_tds_onvif_SystemRebootResponse &tds_onvif_SystemRebootResponse);
    bool device_GetServices(ClientInfo clientInfo, _tds__GetServices *tds__GetServices, _tds__GetServicesResponse &tds__GetServicesResponse);
    bool device_GetServiceCapabilities(ClientInfo clientInfo);

    // bool media_CreateProfile(ClientInfo clientInfo, _ns1__CreateProfile *ns1__CreateProfile, _ns1__CreateProfileResponse &ns1__CreateProfileResponse);
    // bool media_DeleteProfile(ClientInfo clientInfo, _ns1__DeleteProfile *ns1__DeleteProfile, _ns1__DeleteProfileResponse &ns1__DeleteProfileResponse);
    // bool media_GetCompatibleMetadataConfigurations(ClientInfo clientInfo, _ns1__GetCompatibleMetadataConfigurations *ns1__GetCompatibleMetadataConfigurations, _ns1__GetCompatibleMetadataConfigurationsResponse &ns1__GetCompatibleMetadataConfigurationsResponse);
    // bool media_GetCompatibleVideoAnalyticsConfigurations(ClientInfo clientInfo, _ns1__GetCompatibleVideoAnalyticsConfigurations *ns1__GetCompatibleVideoAnalyticsConfigurations, _ns1__GetCompatibleVideoAnalyticsConfigurationsResponse &ns1__GetCompatibleVideoAnalyticsConfigurationsResponse);
    // bool media_GetCompatibleVideoEncoderConfigurations(std::string ProfileToken, std::vector<_ns1_onvif_GetCompatibleVideoEncoderConfigurationsResponse> &ns1__GetCompatibleResponse);
    // bool media_GetCompatibleVideoSourceConfigurations(ClientInfo clientInfo, _ns1__GetCompatibleVideoSourceConfigurations *ns1__GetCompatibleVideoSourceConfigurations, _ns1__GetCompatibleVideoSourceConfigurationsResponse &ns1__GetCompatibleVideoSourceConfigurationsResponse);
    // bool media_GetMetadataConfiguration(ClientInfo clientInfo, _ns1__GetMetadataConfiguration *ns1__GetMetadataConfiguration, _ns1__GetMetadataConfigurationResponse &ns1__GetMetadataConfigurationResponse);
    // bool media_GetMetadataConfigurationOptions(ClientInfo clientInfo, _ns1__GetMetadataConfigurationOptions *ns1__GetMetadataConfigurationOptions, _ns1__GetMetadataConfigurationOptionsResponse &ns1__GetMetadataConfigurationOptionsResponse);
    // bool media_GetMetadataConfigurations(ClientInfo clientInfo, _ns1__GetMetadataConfigurations *ns1__GetMetadataConfigurations, _ns1__GetMetadataConfigurationsResponse &ns1__GetMetadataConfigurationsResponse);
    // bool media_GetProfile(ClientInfo clientInfo, _ns1__GetProfile *ns1__GetProfile, _ns1__GetProfileResponse &ns1__GetProfileResponse);
    bool media_GetProfiles(std::vector<_ns1_onvif_Profile> &ns1_onvif_Profiles);
    // bool media_GetSnapshotUri(std::string ProfileToken, _ns1_onvif_GetSnapshotUriResponse& ns1_onvif_GetSnapshotUriResponse);
    bool media_GetStreamUri(_ns1_onvif_GetStreamUri ns1_onvif_GetStreamUri, _ns1_onvif_GetStreamUriResponse& ns1_onvif_GetStreamUriResponse);
    // bool media_GetVideoAnalyticsConfiguration(ClientInfo clientInfo, _ns1__GetVideoAnalyticsConfiguration *ns1__GetVideoAnalyticsConfiguration, _ns1__GetVideoAnalyticsConfigurationResponse &ns1__GetVideoAnalyticsConfigurationResponse);
    // bool media_GetVideoAnalyticsConfigurations(ClientInfo clientInfo, _ns1__GetVideoAnalyticsConfigurations *ns1__GetVideoAnalyticsConfigurations, _ns1__GetVideoAnalyticsConfigurationsResponse &ns1__GetVideoAnalyticsConfigurationsResponse);
    // bool media_GetVideoEncoderConfigurationOptions(_ns1_onvif_GetVideoEncoderConfigurationOptions ns1_onvif_GetVideoEncoderConfigurationOptions, _ns1_onvif_GetVideoEncoderConfigurationOptionsResponse &ns1_onvif_GetVideoEncoderConfigurationOptionsResponse);
    bool media_GetVideoEncoderConfigurations(_ns1_onvif_GetVideoEncoderConfiguration ns1_onvif_GetVideoEncoderConfiguration, std::vector <_ns1_onvif_VideoEncoderConfiguration> &ns1_onvif_VideoEncoderConfigurations);
    // bool media_GetVideoSourceConfiguration(ClientInfo clientInfo, _ns1__GetVideoSourceConfiguration *ns1__GetVideoSourceConfiguration, _ns1__GetVideoSourceConfigurationResponse &ns1__GetVideoSourceConfigurationResponse);
    // bool media_GetVideoSourceConfigurationOptions(ClientInfo clientInfo, _ns1__GetVideoSourceConfigurationOptions *ns1__GetVideoSourceConfigurationOptions, _ns1__GetVideoSourceConfigurationOptionsResponse &ns1__GetVideoSourceConfigurationOptionsResponse);
    // bool media_GetVideoSourceConfigurations(ClientInfo clientInfo, _ns1__GetVideoSourceConfigurations *ns1__GetVideoSourceConfigurations, _ns1__GetVideoSourceConfigurationsResponse &ns1__GetVideoSourceConfigurationsResponse);
    // bool media_GetVideoSources(ClientInfo clientInfo, _ns1__GetVideoSources *ns1__GetVideoSources, _ns1__GetVideoSourcesResponse &ns1__GetVideoSourcesResponse);
    // bool media_SetMetadataConfiguration(ClientInfo clientInfo, _ns1__SetMetadataConfiguration *ns1__SetMetadataConfiguration, _ns1__SetMetadataConfigurationResponse &ns1__SetMetadataConfigurationResponse);
    // bool media_SetSynchronizationPoint(ClientInfo clientInfo, _ns1__SetSynchronizationPoint *ns1__SetSynchronizationPoint, _ns1__SetSynchronizationPointResponse &ns1__SetSynchronizationPointResponse);
    // bool media_SetVideoAnalyticsConfiguration(ClientInfo clientInfo, _ns1__SetVideoAnalyticsConfiguration *ns1__SetVideoAnalyticsConfiguration, _ns1__SetVideoAnalyticsConfigurationResponse &ns1__SetVideoAnalyticsConfigurationResponse);
    bool media_SetVideoEncoderConfiguration(_ns1_onvif_VideoEncoderConfiguration ns1_onvif_VideoEncoderConfiguration);
    // bool media_SetVideoSourceConfiguration(ClientInfo clientInfo, _ns1__SetVideoSourceConfiguration *ns1__SetVideoSourceConfiguration, _ns1__SetVideoSourceConfigurationResponse &ns1__SetVideoSourceConfigurationResponse);
    // bool media_GetServiceCapabilities(ClientInfo clientInfo, _ns1__GetServiceCapabilities *ns1__GetServiceCapabilities, _ns1__GetServiceCapabilitiesResponse &ns1__GetServiceCapabilitiesResponse);

    bool imaging_GetImagingSettings(ClientInfo clientInfo, _timg__GetImagingSettings *timg__GetImagingSettings, _timg__GetImagingSettingsResponse &timg__GetImagingSettingsResponse);
    bool imaging_GetMoveOptions(ClientInfo clientInfo, _timg__GetMoveOptions *timg__GetMoveOptions, _timg__GetMoveOptionsResponse &timg__GetMoveOptionsResponse);
    bool imaging_GetOptions(ClientInfo clientInfo, _timg__GetOptions *timg__GetOptions, _timg__GetOptionsResponse &timg__GetOptionsResponse);
    bool imaging_GetStatus(ClientInfo clientInfo, _timg__GetStatus *timg__GetStatus, _timg__GetStatusResponse &timg__GetStatusResponse);
    bool imaging_Move(ClientInfo clientInfo, _timg__Move *timg__Move, _timg__MoveResponse &timg__MoveResponse);
    bool imaging_SetImagingSettings(ClientInfo clientInfo, _timg__SetImagingSettings *timg__SetImagingSettings, _timg__SetImagingSettingsResponse &timg__SetImagingSettingsResponse);
    bool imaging_Stop(ClientInfo clientInfo, _timg__Stop *timg__Stop, _timg__StopResponse &timg__StopResponse);
    bool imaging_GetServiceCapabilities(ClientInfo clientInfo, _timg__GetServiceCapabilities *timg__GetServiceCapabilities, _timg__GetServiceCapabilitiesResponse &timg__GetServiceCapabilitiesResponse);

    bool ptz_AbsoluteMove(ClientInfo clientInfo, _tptz__AbsoluteMove *tptz__AbsoluteMove, _tptz__AbsoluteMoveResponse &tptz__AbsoluteMoveResponse);
    bool ptz_ContinuousMove(ClientInfo clientInfo, _tptz__ContinuousMove *tptz__ContinuousMove, _tptz__ContinuousMoveResponse &tptz__ContinuousMoveResponse);
    bool ptz_GetConfiguration(ClientInfo clientInfo, _tptz__GetConfiguration *tptz__GetConfiguration, _tptz__GetConfigurationResponse &tptz__GetConfigurationResponse);
    bool ptz_GetConfigurationOptions(ClientInfo clientInfo, _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, _tptz__GetConfigurationOptionsResponse &tptz__GetConfigurationOptionsResponse);
    bool ptz_GetConfigurations(ClientInfo clientInfo, _tptz__GetConfigurations *tptz__GetConfigurations, _tptz__GetConfigurationsResponse &tptz__GetConfigurationsResponse);
    bool ptz_GetNode(ClientInfo clientInfo, _tptz__GetNode *tptz__GetNode, _tptz__GetNodeResponse &tptz__GetNodeResponse);
    bool ptz_GetNodes(ClientInfo clientInfo, _tptz__GetNodes *tptz__GetNodes, _tptz__GetNodesResponse &tptz__GetNodesResponse);
    bool ptz_GetPresets(ClientInfo clientInfo, _tptz__GetPresets *tptz__GetPresets, _tptz__GetPresetsResponse &tptz__GetPresetsResponse);
    bool ptz_GetStatus(ClientInfo clientInfo, _tptz__GetStatus *tptz__GetStatus, _tptz__GetStatusResponse &tptz__GetStatusResponse);
    bool ptz_GotoHomePosition(ClientInfo clientInfo, _tptz__GotoHomePosition *tptz__GotoHomePosition, _tptz__GotoHomePositionResponse &tptz__GotoHomePositionResponse);
    bool ptz_GotoPreset(ClientInfo clientInfo, _tptz__GotoPreset *tptz__GotoPreset, _tptz__GotoPresetResponse &tptz__GotoPresetResponse);
    bool ptz_RelativeMove(ClientInfo clientInfo, _tptz__RelativeMove *tptz__RelativeMove, _tptz__RelativeMoveResponse &tptz__RelativeMoveResponse);
    bool ptz_RemovePreset(ClientInfo clientInfo, _tptz__RemovePreset *tptz__RemovePreset, _tptz__RemovePresetResponse &tptz__RemovePresetResponse);
    bool ptz_SetConfiguration(ClientInfo clientInfo, _tptz__SetConfiguration *tptz__SetConfiguration, _tptz__SetConfigurationResponse &tptz__SetConfigurationResponse);
    bool ptz_SetHomePosition(ClientInfo clientInfo, _tptz__SetHomePosition *tptz__SetHomePosition, _tptz__SetHomePositionResponse &tptz__SetHomePositionResponse);
    bool ptz_SetPreset(ClientInfo clientInfo, _tptz__SetPreset *tptz__SetPreset, _tptz__SetPresetResponse &tptz__SetPresetResponse);
    bool ptz_Stop(ClientInfo clientInfo, _tptz__Stop *tptz__Stop, _tptz__StopResponse &tptz__StopResponse);
    bool ptz_GetPresetTour(ClientInfo clientInfo, _tptz__GetPresetTour *tptz__GetPresetTour, _tptz__GetPresetTourResponse &tptz__GetPresetTourResponse);
    bool ptz_GetPresetTours(ClientInfo clientInfo, _tptz__GetPresetTours *tptz__GetPresetTours, _tptz__GetPresetToursResponse &tptz__GetPresetToursResponse);
    bool ptz_GetPresetTourOptions(ClientInfo clientInfo, _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, _tptz__GetPresetTourOptionsResponse &tptz__GetPresetTourOptionsResponse);
    bool ptz_CreatePresetTour(ClientInfo clientInfo, _tptz__CreatePresetTour *tptz__CreatePresetTour, _tptz__CreatePresetTourResponse &tptz__CreatePresetTourResponse);
    bool ptz_ModifyPresetTour(ClientInfo clientInfo, _tptz__ModifyPresetTour *tptz__ModifyPresetTour, _tptz__ModifyPresetTourResponse &tptz__ModifyPresetTourResponse);
    bool ptz_OperatePresetTour(ClientInfo clientInfo, _tptz__OperatePresetTour *tptz__OperatePresetTour, _tptz__OperatePresetTourResponse &tptz__OperatePresetTourResponse);
    bool ptz_RemovePresetTour(ClientInfo clientInfo, _tptz__RemovePresetTour *tptz__RemovePresetTour, _tptz__RemovePresetTourResponse &tptz__RemovePresetTourResponse);
    bool ptz_GetServiceCapabilities(ClientInfo clientInfo, _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, _tptz__GetServiceCapabilitiesResponse &tptz__GetServiceCapabilitiesResponse);

    // bool tev_Notify(ClientInfo clientInfo, _tev__Notify *tev__Notify, _tev__NotifyResponse &tev__NotifyResponse);
    // bool tev_Subscribe(ClientInfo clientInfo, _tev__Subscribe *tev__Subscribe, _tev__SubscribeResponse &tev__SubscribeResponse);
    bool tev_Renew(std::string Adress, std::string TerminationTime);
    // bool tev_Unsubscribe(ClientInfo clientInfo, _tev__Unsubscribe *tev__Unsubscribe, _tev__UnsubscribeResponse &tev__UnsubscribeResponse);
    bool tev_SetSynchronizationPoint(ClientInfo clientInfo, _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, _tev__SetSynchronizationPointResponse &tev__SetSynchronizationPointResponse);
    bool tev_CreatePullPointSubscription(std::string InitialTerminationTime, std::string& Address);
    bool tev_PullMessages(std::string Address, _tev_onvif_PullMessages tev_onvif_PullMessages, _tev_onvif_PullMessagesResponse& tev_onvif_PullMessagesResponse);
    bool tev_GetEventProperties(_tev_onvif_GetEventPropertiesResponse &tev_onvif_GetEventPropertiesResponse);
    // bool tev_TopicFilter(ClientInfo clientInfo, _tev__TopicFilter *tev__TopicFilter, _tev__TopicFilterResponse &tev__TopicFilterResponse);
    // bool tev_MessageContentFilter(ClientInfo clientInfo, _tev__MessageContentFilter *tev__MessageContentFilter, _tev__MessageContentFilterResponse &tev__MessageContentFilterResponse);


    ~OnvifApi();
};



#endif  //OnvifApi_H