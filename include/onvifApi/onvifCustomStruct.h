#ifndef ONVIF_CUSTOM_STRUCT_H
#define ONVIF_CUSTOM_STRUCT_H
#include <iostream>

struct _tds_onvif_GetDeviceInformationResponse{
    std::string Manufacturer;
    std::string Model;
    std::string FirmwareVersion;
    std::string SerialNumber;
    std::string HardwareId;
};

struct _tds_onvif_SystemRebootResponse{
    std::string Message;
};

struct _tds_onvif_IPv4NetworkInterface{
    bool Enabled;
    bool DHCP;
    std::string Address;
};

struct _tds_onvif_GetNetworkInterfacesResponse{
    std::string token;
    bool Enabled;
    std::string HwAddress;
    _tds_onvif_IPv4NetworkInterface* IPv4;
};

enum _tt_onvif_VideoEncoding{
    _tt_onvif_VideoResolution_JPEG = 0,
    _tt_onvif_VideoResolution_MPEG4 = 1,
    _tt_onvif_VideoResolution_H264 = 2
};

struct _tt_onvif_VideoResolution{
    int Width;
    int Height;
};

struct _tt_onvif_VideoRateControl{
    int FrameRateLimit;
    bool ConstantBitRate;
    int BitrateLimit;
};

struct _ns1_onvif_GetVideoEncoderConfiguration{
    std::string* ConfigurationToken;
    std::string* ProfileToken;
};

struct _ns1_onvif_VideoEncoderConfiguration{
    std::string token;
    _tt_onvif_VideoResolution Resolution;
    float Quality;
    _tt_onvif_VideoRateControl RateControl;
    int* GovLength;
    std::string Profile;
    std::string Encoding;
};

struct _ns1_onvif_Profile{
    std::string token;
    bool fixed;
    std::string VideoSourceConfigurationToken;
    std::string VideoEncoderConfigurationToken;
    std::string VideoEncoderConfigurationEncoding;
};

// enum _ns1_onvif_StreamType{
//     ns1_StreamType_RTP_Unicast = 0,
//     ns1_StreamType_RTP_Multicast = 1
// };

// enum _ns1_onvif_TransportProtocol{
//     ns1_TransportProtocol_UDP = 0,
//     ns1_TransportProtocol_TCP = 1,
//     ns1_TransportProtocol_RTSP = 2,
//     ns1_TransportProtocol_HTTP = 3
// };

struct _ns1_onvif_GetStreamUri{
    std::string Protocol;
    std::string ProfileToken;
};

struct _ns1_onvif_GetStreamUriResponse{
    std::string Uri;
};

// struct _ns1_onvif_GetSnapshotUriResponse{
//     std::string Uri;
//     bool InvalidAfterConnect;
//     bool InvalidAfterReboot;
// };

// struct _ns1_onvif_GetCompatibleVideoEncoderConfigurationsResponse{
//     std::string token;
// };

// struct _ns1_onvif_GetVideoEncoderConfigurationOptions{
//     std::string ConfigurationToken;
//     std::string ProfileToken; 
// };

struct _tt_onvif_JpegOptions{
    std::vector<_tt_onvif_VideoResolution> ResolutionsAvailable;
};

struct _tt_onvif_Mpeg4Options{
    std::vector<_tt_onvif_VideoResolution> ResolutionsAvailable;
};

struct _tt_onvif_H264Options{
    std::vector<_tt_onvif_VideoResolution> ResolutionsAvailable;
};

struct _tt_onvif_IntRange{
    int Min;
    int Max;
};

// struct _ns1_onvif_GetVideoEncoderConfigurationOptionsResponse{
//     _tt_onvif_IntRange QualityRange;
//     _tt_onvif_JpegOptions JPEG;
//     _tt_onvif_Mpeg4Options MPEG4;
//     _tt_onvif_H264Options H264;
// };

enum _tds_onvif_DiscoveryMode{
    tt_onvif_Discoverable = 0,
    tt_onvif_NonDiscoverable = 1
};

enum _tds_onvif_ScopeDef{
    tt_onvif_ScopeDefinition_Fixed = 0,
    tt_onvif_ScopeDefinition_Configurable
};

struct _tds_onvif_Scope{
    _tds_onvif_ScopeDef ScopeDef;
    std::string ScopeItem;
};

struct _tds_onvif_NetworkGateway{
    std::vector<std::string> IPv4Address;
    std::vector<std::string> IPv6Address;
};

enum _tds_onvif_NetworkProtocolType{
    tt_onvif_NetworkProtocolType_HTTP = 0,
    tt_onvif_NetworkProtocolType_HTTPS,
    tt_onvif_NetworkProtocolType_RTSP
};

struct _tds_onvif_NetworkProtocol{
    _tds_onvif_NetworkProtocolType Name;
    bool Enabled;
    std::vector<int> Port;
};

struct _tds_onvif_HostnameInformation{
    bool FromDHCP;
    std::string Name;
};

enum _tds_onvif_DateTimeType{
    tt_onvif_DateTimeType_Manual = 0,
    tt_onvif_DateTimeType_NTP
};

struct _tds_onvif_Time{
    int Hour;
    int Minute;
    int Second;
};

struct _tds_onvif_Date{
    int Year;
    int Month;
    int Day;
};

struct _tds_onvif_DateTime{
    _tds_onvif_Time Time;
    _tds_onvif_Date Date;
};

struct _tds_onvif_SystemDateTime{
    _tds_onvif_DateTimeType DateTimeType;
    bool DaylightSavings;
    std::string TZ;
    _tds_onvif_DateTime UTCDateTime;
    _tds_onvif_DateTime LocalDateTime;
};

enum _tds_onvif_FactoryDefault{
    tt_onvif_FactoryDefault_Hard = 0,
    tt_onvif_FactoryDefault_Soft
};

enum _tds_onvif_IPType{
    tt_onvif_IPType_IPv4 = 0,
    tt_onvif_IPType_IPv6
};

struct _tds_onvif_IPAddress{
    _tds_onvif_IPType Type;
    std::string IPv4Address;
    std::string IPv6Address;
};

struct _tds_onvif_DNSInformation{
    bool FromDHCP;
    std::vector<std::string> SearchDomain;
    std::vector<_tds_onvif_IPAddress> DNSFromDHCP;
    std::vector<_tds_onvif_IPAddress> DNSManual;
};

struct _tds_onvif_SetSystemDateAndTime{
    _tds_onvif_DateTimeType DateTimeType;
    bool DaylightSavings;
    int TZ = 10;
    _tds_onvif_DateTime UTCDateTime;
};

struct _tds_onvif_PrefixedIPv4Address{
    std::string Address;
    int PrefixLength;
};

struct _tds_onvif_IPv4NetworkInterfaceSetConfiguration{
    bool DHCP;
    std::vector<_tds_onvif_PrefixedIPv4Address> Manual;
};

struct _tds_onvif_SetNetworkInterfaces{
    std::string InterfaceToken;
    _tds_onvif_IPv4NetworkInterfaceSetConfiguration IPv4;
};

enum _tds_onvif_LogType{
    tt_onvif_LogType_System = 0,
    tt_onvif_LogType_Access
};

struct _tev_onvif_GetEventPropertiesResponse{
    std::string TopicNamespaceLocation;
    bool FixedTopicSet;
    std::vector<std::string> TopicSet;
    std::string TopicExpressionDialect;
    std::string MessageContentFilterDialect;
    std::string ProducerPropertiesFilterDialect;
    std::string MessageContentSchemaLocation;
};

struct _tev_onvif_PullMessages{
    std::string Timeout;
    int MessageLimit;
};

struct _tev_onvif_NotificationMessage{
    std::string Topic;
    std::string Message;
};

struct _tev_onvif_PullMessagesResponse{
    std::string CurrentTime;
    std::string TerminationTime;
    std::vector<_tev_onvif_NotificationMessage> NotificationMessage;
};




#endif