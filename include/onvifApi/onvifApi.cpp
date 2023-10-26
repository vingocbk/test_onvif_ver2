#include "onvifApi.h"


OnvifApi::OnvifApi(std::string ip, int port, std::string username, std::string password, bool useHttps)
{
    struct soap *soap = soap_new1(SOAP_XML_STRICT | SOAP_XML_CANONICAL);
    soap->connect_timeout = soap->recv_timeout = soap->send_timeout = 10; // 10 sec
    soap_register_plugin(soap, soap_wsse);
    this->soap = soap_copy(soap);
    this->clientInfo.ip = ip;
    this->clientInfo.port = port;
    this->clientInfo.username = username;
    this->clientInfo.password = password;
    this->clientInfo.useHttps = useHttps;
    device_GetServiceCapabilities(this->clientInfo);
}

OnvifApi::OnvifApi(struct soap *soap)
{
    this->soap = soap_copy(soap);
}

OnvifApi::~OnvifApi()
{
    soap_destroy(this->soap);
    soap_end(this->soap);
    soap_free(this->soap);
}

// void OnvifApi::init()
// {
//     struct soap *soap = soap_new1(SOAP_XML_STRICT | SOAP_XML_CANONICAL);
//     soap->connect_timeout = soap->recv_timeout = soap->send_timeout = 10; // 10 sec
//     soap_register_plugin(soap, soap_wsse);
//     this->soap = soap_copy(soap);
// }

void OnvifApi::set_credentials(std::string username, std::string password, bool isDigestLogin)
{
    if(username.empty() && password.empty())
    {
        return;
    }
    soap_wsse_delete_Security(this->soap);
    if (isDigestLogin) {
        if (soap_wsse_add_Timestamp(this->soap, "Time", 10)
        || soap_wsse_add_UsernameTokenDigest(this->soap, "Auth", username.c_str(), password.c_str()))
            // || soap_wsse_add_UsernameTokenText(this->soap, "Auth", username.c_str(), password.c_str()))
        {
            report_error();
        }
    }
    else {
        if (soap_wsse_add_Timestamp(this->soap, "Time", 10)
        // || soap_wsse_add_UsernameTokenDigest(this->soap, "Auth", username.c_str(), password.c_str()))
            || soap_wsse_add_UsernameTokenText(this->soap, "Auth", username.c_str(), password.c_str()))
        {
            report_error();
        }
    }
}

void OnvifApi::report_error()
{
    std::cerr << "Oops, something went wrong:" << std::endl;
    soap_stream_fault(soap, std::cerr);
    // exit(EXIT_FAILURE);
}

void OnvifApi::changeClientInfo(std::string ip, int port, std::string username, std::string password, bool useHttps)
{
    this->clientInfo.ip = ip;
    this->clientInfo.port = port;
    this->clientInfo.username = username;
    this->clientInfo.password = password;
    this->clientInfo.useHttps = useHttps;
}

std::string OnvifApi::getEndpointDevice()
{
    std::string endpoint;
    if(this->clientInfo.useHttps){
        // enable https connections with server certificate verification using cacerts.pem
        if (soap_ssl_client_context(soap, SOAP_SSL_SKIP_HOST_CHECK, NULL, NULL, "../../libOnvif/cacerts.pem", NULL, NULL))
            report_error();
        endpoint = "https://" + this->clientInfo.ip + ":" + std::to_string(this->clientInfo.port) + "/onvif/device_service";
    }
    else{
        endpoint = "http://" + this->clientInfo.ip + ":" + std::to_string(this->clientInfo.port) + "/onvif/device_service";
    }
    
    return endpoint.c_str();
}

std::string OnvifApi::getEndpointMedia()
{
    std::string endpoint;
    _tds__GetServices GetServices;
    _tds__GetServicesResponse GetServicesResponse;
    GetServices.IncludeCapability = false;
    if(device_GetServices(this->clientInfo, &GetServices, GetServicesResponse)){
        for(size_t i = 0; i < GetServicesResponse.Service.size(); i++){
            std::string Namespace = GetServicesResponse.Service[i]->Namespace;
            if (Namespace.find("/media/wsdl") != std::string::npos) {
                // endpoint = GetServicesResponse.Service[i]->XAddr;
                std::string XAddr = GetServicesResponse.Service[i]->XAddr;
                endpoint = "http://" + this->clientInfo.ip + ":" + std::to_string(clientInfo.port);
                endpoint += XAddr.substr(XAddr.find("/onvif"), XAddr.length());
                // std::cout << "endpoint:-" <<  endpoint << "-" << std::endl;
                break;
            } 
        }
    }
    return endpoint.c_str();
}

std::string OnvifApi::getEndpointImaging()
{
    std::string endpoint;
    _tds__GetServices GetServices;
    _tds__GetServicesResponse GetServicesResponse;
    GetServices.IncludeCapability = false;
    if(device_GetServices(this->clientInfo, &GetServices, GetServicesResponse)){
        for(size_t i = 0; i < GetServicesResponse.Service.size(); i++){
            std::string Namespace = GetServicesResponse.Service[i]->Namespace;
            if (Namespace.find("/imaging/wsdl") != std::string::npos) {
                // endpoint = GetServicesResponse.Service[i]->XAddr;
                std::string XAddr = GetServicesResponse.Service[i]->XAddr;
                endpoint = "http://" + this->clientInfo.ip + ":" + std::to_string(clientInfo.port);
                endpoint += XAddr.substr(XAddr.find("/onvif"), XAddr.length());
                // std::cout << "endpoint:-" <<  endpoint << "-" << std::endl;
                break;
            } 
        }
    }
    return endpoint.c_str();
}

std::string OnvifApi::getEndpointPtz()
{
    std::string endpoint;
    _tds__GetServices GetServices;
    _tds__GetServicesResponse GetServicesResponse;
    GetServices.IncludeCapability = false;
    if(device_GetServices(this->clientInfo, &GetServices, GetServicesResponse)){
        for(size_t i = 0; i < GetServicesResponse.Service.size(); i++){
            std::string Namespace = GetServicesResponse.Service[i]->Namespace;
            if (Namespace.find("/ptz/wsdl") != std::string::npos) {
                // endpoint = GetServicesResponse.Service[i]->XAddr;
                std::string XAddr = GetServicesResponse.Service[i]->XAddr;
                endpoint = "http://" + this->clientInfo.ip + ":" + std::to_string(clientInfo.port);
                endpoint += XAddr.substr(XAddr.find("/onvif"), XAddr.length());
                // std::cout << "endpoint:-" <<  endpoint << "-" << std::endl;
                break;
            } 
        }
    }
    return endpoint.c_str();
}

std::string OnvifApi::getEndpointEvent()
{
    std::string endpoint;
    _tds__GetServices GetServices;
    _tds__GetServicesResponse GetServicesResponse;
    GetServices.IncludeCapability = false;
    if(device_GetServices(this->clientInfo, &GetServices, GetServicesResponse)){
        for(size_t i = 0; i < GetServicesResponse.Service.size(); i++){
            std::string Namespace = GetServicesResponse.Service[i]->Namespace;
            if (Namespace.find("/events/wsdl") != std::string::npos) {
                // endpoint = GetServicesResponse.Service[i]->XAddr;
                std::string XAddr = GetServicesResponse.Service[i]->XAddr;
                endpoint = "http://" + this->clientInfo.ip + ":" + std::to_string(clientInfo.port);
                endpoint += XAddr.substr(XAddr.find("/onvif"), XAddr.length());
                // std::cout << "endpoint:-" <<  endpoint << "-" << std::endl;
                break;
            } 
        }
    }
    return endpoint.c_str();
}

bool OnvifApi::device_AddScopes(std::vector<std::string> ScopeItem)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__AddScopes tds__AddScopes;
    for(long unsigned int i = 0; i < ScopeItem.size(); i++){
        tds__AddScopes.ScopeItem.push_back(ScopeItem[i].c_str());
    }
    _tds__AddScopesResponse tds__AddScopesResponse;
    if (proxyDevice.AddScopes(&tds__AddScopes, tds__AddScopesResponse))
    {
        report_error();
        return false;
    }
    return true;
}


bool OnvifApi::device_CreateUsers(_tds__CreateUsers *tds__CreateUsers, _tds__CreateUsersResponse &tds__CreateUsersResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.CreateUsers(tds__CreateUsers, tds__CreateUsersResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_DeleteUsers(ClientInfo clientInfo, _tds__DeleteUsers *tds__DeleteUsers, _tds__DeleteUsersResponse &tds__DeleteUsersResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.DeleteUsers(tds__DeleteUsers, tds__DeleteUsersResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_GetDNS(_tds_onvif_DNSInformation& tds_onvif_DNSInformation)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetDNS tds__GetDNS;
    _tds__GetDNSResponse tds__GetDNSResponse;
    if (proxyDevice.GetDNS(&tds__GetDNS, tds__GetDNSResponse))
    {
        report_error();
        return false;
    }
    tds_onvif_DNSInformation.FromDHCP = tds__GetDNSResponse.DNSInformation->FromDHCP;
    for(long unsigned int i = 0; i < tds__GetDNSResponse.DNSInformation->SearchDomain.size(); i++){
        tds_onvif_DNSInformation.SearchDomain.push_back(tds__GetDNSResponse.DNSInformation->SearchDomain[i]);
    }
    for(long unsigned int i = 0; i < tds__GetDNSResponse.DNSInformation->DNSFromDHCP.size(); i++){
        _tds_onvif_IPAddress data;
        data.Type = (_tds_onvif_IPType)tds__GetDNSResponse.DNSInformation->DNSFromDHCP[i]->Type;
        if(tds__GetDNSResponse.DNSInformation->DNSFromDHCP[i]->IPv4Address){
            data.IPv4Address = *tds__GetDNSResponse.DNSInformation->DNSFromDHCP[i]->IPv4Address;
        }
        if(tds__GetDNSResponse.DNSInformation->DNSFromDHCP[i]->IPv6Address){
            data.IPv6Address = *tds__GetDNSResponse.DNSInformation->DNSFromDHCP[i]->IPv6Address;
        }
        tds_onvif_DNSInformation.DNSFromDHCP.push_back(data);
    }
    for(long unsigned int i = 0; i < tds__GetDNSResponse.DNSInformation->DNSManual.size(); i++){
        _tds_onvif_IPAddress data;
        data.Type = (_tds_onvif_IPType)tds__GetDNSResponse.DNSInformation->DNSManual[i]->Type;
        if(tds__GetDNSResponse.DNSInformation->DNSManual[i]->IPv4Address){
            data.IPv4Address = *tds__GetDNSResponse.DNSInformation->DNSManual[i]->IPv4Address;
        }
        if(tds__GetDNSResponse.DNSInformation->DNSManual[i]->IPv6Address){
            data.IPv6Address = *tds__GetDNSResponse.DNSInformation->DNSManual[i]->IPv6Address;
        }
        tds_onvif_DNSInformation.DNSManual.push_back(data);
    }
    return true;
}

bool OnvifApi::device_GetCapabilities(ClientInfo clientInfo, _tds__GetCapabilities *tds__GetCapabilities, _tds__GetCapabilitiesResponse &tds__GetCapabilitiesResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.GetCapabilities(tds__GetCapabilities, tds__GetCapabilitiesResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_GetDeviceInformation(_tds_onvif_GetDeviceInformationResponse &tds_onvif_GetDeviceInformationResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetDeviceInformation tds__GetDeviceInformation;
    _tds__GetDeviceInformationResponse tds__GetDeviceInformationResponse;
    if (proxyDevice.GetDeviceInformation(&tds__GetDeviceInformation, tds__GetDeviceInformationResponse))
    {
        report_error();
        return false;
    }
    tds_onvif_GetDeviceInformationResponse.Manufacturer = tds__GetDeviceInformationResponse.Manufacturer;
    tds_onvif_GetDeviceInformationResponse.Model = tds__GetDeviceInformationResponse.Model;
    tds_onvif_GetDeviceInformationResponse.FirmwareVersion = tds__GetDeviceInformationResponse.FirmwareVersion;
    tds_onvif_GetDeviceInformationResponse.SerialNumber = tds__GetDeviceInformationResponse.SerialNumber;
    tds_onvif_GetDeviceInformationResponse.HardwareId = tds__GetDeviceInformationResponse.HardwareId;
    return true;
}

bool OnvifApi::device_GetDiscoveryMode(_tds_onvif_DiscoveryMode &tds_onvif_DiscoveryMode)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetDiscoveryMode tds__GetDiscoveryMode;
    _tds__GetDiscoveryModeResponse tds__GetDiscoveryModeResponse;
    if (proxyDevice.GetDiscoveryMode(&tds__GetDiscoveryMode, tds__GetDiscoveryModeResponse))
    {
        report_error();
        return false;
    }
    tds_onvif_DiscoveryMode = (_tds_onvif_DiscoveryMode)tds__GetDiscoveryModeResponse.DiscoveryMode;
    return true;
}

bool OnvifApi::device_GetHostname(_tds_onvif_HostnameInformation& tds_onvif_HostnameInformation)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetHostname tds__GetHostname;
    _tds__GetHostnameResponse tds__GetHostnameResponse;
    if (proxyDevice.GetHostname(&tds__GetHostname, tds__GetHostnameResponse))
    {
        report_error();
        return false;
    }
    tds_onvif_HostnameInformation.FromDHCP = tds__GetHostnameResponse.HostnameInformation->FromDHCP;
    if(tds__GetHostnameResponse.HostnameInformation->Name){
        tds_onvif_HostnameInformation.Name = *tds__GetHostnameResponse.HostnameInformation->Name;
    }
    return true;
}

bool OnvifApi::device_GetNetworkDefaultGateway(_tds_onvif_NetworkGateway& tds_onvif_NetworkGateway)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetNetworkDefaultGateway tds__GetNetworkDefaultGateway;
    _tds__GetNetworkDefaultGatewayResponse tds__GetNetworkDefaultGatewayResponse;
    if (proxyDevice.GetNetworkDefaultGateway(&tds__GetNetworkDefaultGateway, tds__GetNetworkDefaultGatewayResponse))
    {
        report_error();
        return false;
    }
    for(long unsigned int i = 0; i < tds__GetNetworkDefaultGatewayResponse.NetworkGateway->IPv4Address.size(); i++){
        tds_onvif_NetworkGateway.IPv4Address.push_back(tds__GetNetworkDefaultGatewayResponse.NetworkGateway->IPv4Address[i]);
    }
    for(long unsigned int i = 0; i < tds__GetNetworkDefaultGatewayResponse.NetworkGateway->IPv6Address.size(); i++){
        tds_onvif_NetworkGateway.IPv6Address.push_back(tds__GetNetworkDefaultGatewayResponse.NetworkGateway->IPv6Address[i]);
    }
    return true;
}

bool OnvifApi::device_GetNetworkInterfaces(std::vector<_tds_onvif_GetNetworkInterfacesResponse> &tds_onvif_GetNetworkInterfacesResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetNetworkInterfaces tds__GetNetworkInterfaces;
    _tds__GetNetworkInterfacesResponse tds__GetNetworkInterfacesResponse;
    if (proxyDevice.GetNetworkInterfaces(&tds__GetNetworkInterfaces, tds__GetNetworkInterfacesResponse))
    {
        report_error();
        return false;
    }
    for(long unsigned int i = 0; i < tds__GetNetworkInterfacesResponse.NetworkInterfaces.size(); i++){
        _tds_onvif_GetNetworkInterfacesResponse data;
        data.token = tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->token;
        data.Enabled = tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->Enabled;
        data.HwAddress = tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->Info->HwAddress;
        if(tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->IPv4){
            data.IPv4 = new _tds_onvif_IPv4NetworkInterface();
            data.IPv4->Enabled = tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->IPv4->Enabled;
            if(tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->IPv4->Config){
                data.IPv4->DHCP = tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->IPv4->Config->DHCP;
                if(tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->IPv4->Config->DHCP){
                    data.IPv4->Address = tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->IPv4->Config->FromDHCP->Address;
                }
                else{
                    data.IPv4->Address = tds__GetNetworkInterfacesResponse.NetworkInterfaces[i]->IPv4->Config->Manual.back()->Address;
                }
            }
        }
        tds_onvif_GetNetworkInterfacesResponse.push_back(data);
    }
    return true;
}

bool OnvifApi::device_GetNetworkProtocols(std::vector<_tds_onvif_NetworkProtocol>& tds_onvif_NetworkProtocol)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetNetworkProtocols tds__GetNetworkProtocols;
    _tds__GetNetworkProtocolsResponse tds__GetNetworkProtocolsResponse;
    if (proxyDevice.GetNetworkProtocols(&tds__GetNetworkProtocols, tds__GetNetworkProtocolsResponse))
    {
        report_error();
        return false;
    }
    for(long unsigned int i = 0; i < tds__GetNetworkProtocolsResponse.NetworkProtocols.size(); i++){
        _tds_onvif_NetworkProtocol data;
        data.Enabled = tds__GetNetworkProtocolsResponse.NetworkProtocols[i]->Enabled;
        data.Name = (_tds_onvif_NetworkProtocolType)tds__GetNetworkProtocolsResponse.NetworkProtocols[i]->Name;
        for(long unsigned int j = 0; j < tds__GetNetworkProtocolsResponse.NetworkProtocols[i]->Port.size(); j++){
            data.Port.push_back(tds__GetNetworkProtocolsResponse.NetworkProtocols[i]->Port[j]);
        }
        tds_onvif_NetworkProtocol.push_back(data);
    }
    return true;
}

bool OnvifApi::device_GetNTP(ClientInfo clientInfo, _tds__GetNTP *tds__GetNTP, _tds__GetNTPResponse &tds__GetNTPResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.GetNTP(tds__GetNTP, tds__GetNTPResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_GetScopes(std::vector<_tds_onvif_Scope>& tds_onvif_ScopeResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetScopes tds__GetScopes;
    _tds__GetScopesResponse tds__GetScopesResponse;
    if (proxyDevice.GetScopes(&tds__GetScopes, tds__GetScopesResponse))
    {
        report_error();
        return false;
    }
    for(long unsigned int i = 0; i < tds__GetScopesResponse.Scopes.size(); i++){
        _tds_onvif_Scope data;
        data.ScopeDef = (_tds_onvif_ScopeDef)tds__GetScopesResponse.Scopes[i]->ScopeDef;
        data.ScopeItem = tds__GetScopesResponse.Scopes[i]->ScopeItem;
        tds_onvif_ScopeResponse.push_back(data);
    }

    return true;
}

bool OnvifApi::device_GetSystemDateAndTime(_tds_onvif_SystemDateTime& tds_onvif_SystemDateTime)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetSystemDateAndTime tds__GetSystemDateAndTime;
    _tds__GetSystemDateAndTimeResponse tds__GetSystemDateAndTimeResponse;
    if (proxyDevice.GetSystemDateAndTime(&tds__GetSystemDateAndTime, tds__GetSystemDateAndTimeResponse))
    {
        report_error();
        return false;
    }
    tds_onvif_SystemDateTime.DateTimeType = (_tds_onvif_DateTimeType)tds__GetSystemDateAndTimeResponse.SystemDateAndTime->DateTimeType;
    tds_onvif_SystemDateTime.DaylightSavings = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->DaylightSavings;
    if(tds__GetSystemDateAndTimeResponse.SystemDateAndTime->TimeZone){
        tds_onvif_SystemDateTime.TZ = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->TimeZone->TZ;
    }
    if(tds__GetSystemDateAndTimeResponse.SystemDateAndTime->UTCDateTime){
        tds_onvif_SystemDateTime.UTCDateTime.Time.Hour = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->UTCDateTime->Time->Hour; 
        tds_onvif_SystemDateTime.UTCDateTime.Time.Minute = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->UTCDateTime->Time->Minute; 
        tds_onvif_SystemDateTime.UTCDateTime.Time.Second = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->UTCDateTime->Time->Second;

        tds_onvif_SystemDateTime.UTCDateTime.Date.Year = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->UTCDateTime->Date->Year; 
        tds_onvif_SystemDateTime.UTCDateTime.Date.Month = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->UTCDateTime->Date->Month; 
        tds_onvif_SystemDateTime.UTCDateTime.Date.Day = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->UTCDateTime->Date->Day; 
    }
    if(tds__GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime){
        tds_onvif_SystemDateTime.LocalDateTime.Time.Hour = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Time->Hour; 
        tds_onvif_SystemDateTime.LocalDateTime.Time.Minute = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Time->Minute; 
        tds_onvif_SystemDateTime.LocalDateTime.Time.Second = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Time->Second;

        tds_onvif_SystemDateTime.LocalDateTime.Date.Year = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Date->Year; 
        tds_onvif_SystemDateTime.LocalDateTime.Date.Month = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Date->Month; 
        tds_onvif_SystemDateTime.LocalDateTime.Date.Day = tds__GetSystemDateAndTimeResponse.SystemDateAndTime->LocalDateTime->Date->Day; 
    }
    return true;
}

bool OnvifApi::device_GetSystemLog(_tds_onvif_LogType tds_onvif_LogType, std::string& String)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetSystemLog tds__GetSystemLog;
    tds__GetSystemLog.LogType = (tt__SystemLogType)tds_onvif_LogType;
    _tds__GetSystemLogResponse tds__GetSystemLogResponse;
    if (proxyDevice.GetSystemLog(&tds__GetSystemLog, tds__GetSystemLogResponse))
    {
        report_error();
        return false;
    }
    if(tds__GetSystemLogResponse.SystemLog->String){
        String = *tds__GetSystemLogResponse.SystemLog->String;
    }
    return true;
}

bool OnvifApi::device_GetUsers(ClientInfo clientInfo, _tds__GetUsers *tds__GetUsers, _tds__GetUsersResponse &tds__GetUsersResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.GetUsers(tds__GetUsers, tds__GetUsersResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_GetWsdlUrl(ClientInfo clientInfo, _tds__GetWsdlUrl *tds__GetWsdlUrl, _tds__GetWsdlUrlResponse &tds__GetWsdlUrlResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.GetWsdlUrl(tds__GetWsdlUrl, tds__GetWsdlUrlResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_RemoveScopes(std::vector<std::string> ScopeItem, std::vector<std::string> &ScopeItemResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
     _tds__RemoveScopes tds__RemoveScopes;
     for(long unsigned int i = 0; i < ScopeItem.size(); i++){
        tds__RemoveScopes.ScopeItem.push_back(ScopeItem[i].c_str());
     }
    _tds__RemoveScopesResponse tds__RemoveScopesResponse;
    if (proxyDevice.RemoveScopes(&tds__RemoveScopes, tds__RemoveScopesResponse))
    {
        report_error();
        return false;
    }
    for(long unsigned int i = 0; i < tds__RemoveScopesResponse.ScopeItem.size(); i++){
        ScopeItemResponse.push_back(tds__RemoveScopesResponse.ScopeItem[i]);
    }
    return true;
}

bool OnvifApi::device_SetDiscoveryMode(_tds_onvif_DiscoveryMode tds_onvif_DiscoveryMode)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SetDiscoveryMode tds__SetDiscoveryMode;
    tds__SetDiscoveryMode.DiscoveryMode = (tt__DiscoveryMode)tds_onvif_DiscoveryMode;
    _tds__SetDiscoveryModeResponse tds__SetDiscoveryModeResponse;
    if (proxyDevice.SetDiscoveryMode(&tds__SetDiscoveryMode, tds__SetDiscoveryModeResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetHostname(std::string Name)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SetHostname tds__SetHostname;
    tds__SetHostname.Name = Name;
    _tds__SetHostnameResponse tds__SetHostnameResponse;
    if (proxyDevice.SetHostname(&tds__SetHostname, tds__SetHostnameResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetIPAddressFilter(ClientInfo clientInfo, _tds__SetIPAddressFilter *tds__SetIPAddressFilter, _tds__SetIPAddressFilterResponse &tds__SetIPAddressFilterResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.SetIPAddressFilter(tds__SetIPAddressFilter, tds__SetIPAddressFilterResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetNetworkDefaultGateway(_tds_onvif_NetworkGateway tds_onvif_NetworkGateway)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SetNetworkDefaultGateway tds__SetNetworkDefaultGateway;
    for(long unsigned int i = 0; i < tds_onvif_NetworkGateway.IPv4Address.size(); i++){
        tds__SetNetworkDefaultGateway.IPv4Address.push_back(tds_onvif_NetworkGateway.IPv4Address[i]);
    }
    for(long unsigned int i = 0; i < tds_onvif_NetworkGateway.IPv6Address.size(); i++){
        tds__SetNetworkDefaultGateway.IPv6Address.push_back(tds_onvif_NetworkGateway.IPv6Address[i]);
    }
    _tds__SetNetworkDefaultGatewayResponse tds__SetNetworkDefaultGatewayResponse;
    if (proxyDevice.SetNetworkDefaultGateway(&tds__SetNetworkDefaultGateway, tds__SetNetworkDefaultGatewayResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetNetworkInterfaces(_tds_onvif_SetNetworkInterfaces tds_onvif_SetNetworkInterfaces, bool &RebootNeeded)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SetNetworkInterfaces tds__SetNetworkInterfaces;
    std::vector<_tds_onvif_GetNetworkInterfacesResponse> tds_onvif_GetNetworkInterfacesResponse;
    if(this->device_GetNetworkInterfaces(tds_onvif_GetNetworkInterfacesResponse)){
        tds__SetNetworkInterfaces.InterfaceToken = tds_onvif_GetNetworkInterfacesResponse.front().token;
        // std::cout << tds__SetNetworkInterfaces.InterfaceToken << std::endl;
    }
    tds__SetNetworkInterfaces.NetworkInterface = soap_new_tt__NetworkInterfaceSetConfiguration(this->soap);
    tds__SetNetworkInterfaces.NetworkInterface->IPv4 = soap_new_tt__IPv4NetworkInterfaceSetConfiguration(this->soap);
    tds__SetNetworkInterfaces.NetworkInterface->IPv4->Enabled = new bool(true); 
    tds__SetNetworkInterfaces.NetworkInterface->IPv4->DHCP = new bool(tds_onvif_SetNetworkInterfaces.IPv4.DHCP); 
    if(!tds_onvif_SetNetworkInterfaces.IPv4.DHCP){
        tds__SetNetworkInterfaces.NetworkInterface->IPv4->Manual.push_back(soap_new_tt__PrefixedIPv4Address(this->soap));
        tds__SetNetworkInterfaces.NetworkInterface->IPv4->Manual[0]->Address = tds_onvif_SetNetworkInterfaces.IPv4.Manual.back().Address;
        tds__SetNetworkInterfaces.NetworkInterface->IPv4->Manual[0]->PrefixLength = 1;
        // std::cout << tds_onvif_SetNetworkInterfaces.IPv4.Manual.back().Address << std::endl;
    }
    _tds__SetNetworkInterfacesResponse tds__SetNetworkInterfacesResponse;
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.SetNetworkInterfaces(&tds__SetNetworkInterfaces, tds__SetNetworkInterfacesResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetNetworkProtocols(std::vector<_tds_onvif_NetworkProtocol> tds_onvif_NetworkProtocol)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SetNetworkProtocols tds__SetNetworkProtocols;
    for(long unsigned int i = 0; i < tds_onvif_NetworkProtocol.size(); i++){
        tt__NetworkProtocol *data = soap_new_tt__NetworkProtocol(this->soap);
        data->Name = (tt__NetworkProtocolType)tds_onvif_NetworkProtocol[i].Name;
        data->Enabled = tds_onvif_NetworkProtocol[i].Enabled;
        for(long unsigned j = 0; j < tds_onvif_NetworkProtocol[i].Port.size(); j++){
            data->Port.push_back(tds_onvif_NetworkProtocol[i].Port[j]);
        }
        tds__SetNetworkProtocols.NetworkProtocols.push_back(data);
    }
    _tds__SetNetworkProtocolsResponse tds__SetNetworkProtocolsResponse;
    if (proxyDevice.SetNetworkProtocols(&tds__SetNetworkProtocols, tds__SetNetworkProtocolsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetNTP(ClientInfo clientInfo, _tds__SetNTP *tds__SetNTP, _tds__SetNTPResponse &tds__SetNTPResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.SetNTP(tds__SetNTP, tds__SetNTPResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetScopes(std::vector<std::string> Scopes)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SetScopes tds__SetScopes;
    for(long unsigned int i = 0; i < Scopes.size(); i++){
        tds__SetScopes.Scopes.push_back(Scopes[i].c_str());
    }
    _tds__SetScopesResponse tds__SetScopesResponse;
    if (proxyDevice.SetScopes(&tds__SetScopes, tds__SetScopesResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetSystemDateAndTime(_tds_onvif_SetSystemDateAndTime tds_onvif_SetSystemDateAndTime)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SetSystemDateAndTime tds__SetSystemDateAndTime;
    tds__SetSystemDateAndTime.DateTimeType = (tt__SetDateTimeType)tds_onvif_SetSystemDateAndTime.DateTimeType;
    tds__SetSystemDateAndTime.DaylightSavings = tds_onvif_SetSystemDateAndTime.DaylightSavings;
    if(tds_onvif_SetSystemDateAndTime.TZ != 10){
        tds__SetSystemDateAndTime.TimeZone = soap_new_tt__TimeZone(this->soap);
        tds__SetSystemDateAndTime.TimeZone->TZ = "GMT";  //<+07>-7";
        if(tds_onvif_SetSystemDateAndTime.TZ >= 0){
            tds__SetSystemDateAndTime.TimeZone->TZ += "+";
        }
        tds__SetSystemDateAndTime.TimeZone->TZ += std::to_string(tds_onvif_SetSystemDateAndTime.TZ);
        std::cout << "TZ: " << tds__SetSystemDateAndTime.TimeZone->TZ << std::endl;
    }
    if(tds__SetSystemDateAndTime.DateTimeType == tt__SetDateTimeType__Manual){
        tds__SetSystemDateAndTime.UTCDateTime = soap_new_tt__DateTime(this->soap);
        tds__SetSystemDateAndTime.UTCDateTime->Time = soap_new_tt__Time(this->soap);
        tds__SetSystemDateAndTime.UTCDateTime->Time->Hour = tds_onvif_SetSystemDateAndTime.UTCDateTime.Time.Hour;
        tds__SetSystemDateAndTime.UTCDateTime->Time->Minute = tds_onvif_SetSystemDateAndTime.UTCDateTime.Time.Minute;
        tds__SetSystemDateAndTime.UTCDateTime->Time->Second = tds_onvif_SetSystemDateAndTime.UTCDateTime.Time.Second;
        tds__SetSystemDateAndTime.UTCDateTime->Date = soap_new_tt__Date(this->soap);
        tds__SetSystemDateAndTime.UTCDateTime->Date->Year = tds_onvif_SetSystemDateAndTime.UTCDateTime.Date.Year;
        tds__SetSystemDateAndTime.UTCDateTime->Date->Month = tds_onvif_SetSystemDateAndTime.UTCDateTime.Date.Month;
        tds__SetSystemDateAndTime.UTCDateTime->Date->Day = tds_onvif_SetSystemDateAndTime.UTCDateTime.Date.Day;
    }
    _tds__SetSystemDateAndTimeResponse tds__SetSystemDateAndTimeResponse;
    if (proxyDevice.SetSystemDateAndTime(&tds__SetSystemDateAndTime, tds__SetSystemDateAndTimeResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetSystemFactoryDefault(_tds_onvif_FactoryDefault tds_onvif_FactoryDefault)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SetSystemFactoryDefault tds__SetSystemFactoryDefault;
    tds__SetSystemFactoryDefault.FactoryDefault = (tt__FactoryDefaultType)tds_onvif_FactoryDefault;
    _tds__SetSystemFactoryDefaultResponse tds__SetSystemFactoryDefaultResponse;
    if (proxyDevice.SetSystemFactoryDefault(&tds__SetSystemFactoryDefault, tds__SetSystemFactoryDefaultResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SetUser(ClientInfo clientInfo, _tds__SetUser *tds__SetUser, _tds__SetUserResponse &tds__SetUserResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.SetUser(tds__SetUser, tds__SetUserResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_SystemReboot(_tds_onvif_SystemRebootResponse &tds_onvif_SystemRebootResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__SystemReboot tds__SystemReboot;
    _tds__SystemRebootResponse tds__SystemRebootResponse;
    if (proxyDevice.SystemReboot(&tds__SystemReboot, tds__SystemRebootResponse))
    {
        report_error();
        return false;
    }
    tds_onvif_SystemRebootResponse.Message = tds__SystemRebootResponse.Message;
    return true;
}

bool OnvifApi::device_GetServices(ClientInfo clientInfo, _tds__GetServices *tds__GetServices, _tds__GetServicesResponse &tds__GetServicesResponse)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyDevice.GetServices(tds__GetServices, tds__GetServicesResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::device_GetServiceCapabilities(ClientInfo clientInfo)
{
    DeviceBindingProxy proxyDevice(this->soap);
    std::string endpoint = getEndpointDevice();
    proxyDevice.soap_endpoint = endpoint.c_str();
    // set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tds__GetServiceCapabilities tds__GetServiceCapabilities;
    _tds__GetServiceCapabilitiesResponse tds__GetServiceCapabilitiesResponse;
    if (proxyDevice.GetServiceCapabilities(&tds__GetServiceCapabilities, tds__GetServiceCapabilitiesResponse))
    {
        report_error();
        return false;
    }
    if(tds__GetServiceCapabilitiesResponse.Capabilities->Security->HttpDigest){
        this->clientInfo.isDigestLogin = *tds__GetServiceCapabilitiesResponse.Capabilities->Security->HttpDigest;
    }
    else this->clientInfo.isDigestLogin = false;
    return true;
}

bool OnvifApi::media_GetProfiles(std::vector<_ns1_onvif_Profile> &ns1_onvif_Profiles){
    Media2BindingProxy proxyMedia(this->soap);
    std::string endpoint = getEndpointMedia();
    proxyMedia.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _ns1__GetProfiles ns1__GetProfiles;
    _ns1__GetProfilesResponse ns1__GetProfilesResponse;
    if (proxyMedia.GetProfiles(&ns1__GetProfiles, ns1__GetProfilesResponse))
    {
        report_error();
        return false;
    }
    std::cout << ns1__GetProfilesResponse.Profiles.size() << std::endl;
    for(long unsigned int i = 0; i < ns1__GetProfilesResponse.Profiles.size(); i++){
        _ns1_onvif_Profile data;
        data.token = ns1__GetProfilesResponse.Profiles[i]->token;
        if(ns1__GetProfilesResponse.Profiles[i]->Configurations){
            if(ns1__GetProfilesResponse.Profiles[i]->Configurations->VideoSource){
                data.VideoSourceConfigurationToken = ns1__GetProfilesResponse.Profiles[i]->Configurations->VideoSource->token;
            }
            if(ns1__GetProfilesResponse.Profiles[i]->Configurations->VideoEncoder){
                data.VideoEncoderConfigurationToken = ns1__GetProfilesResponse.Profiles[i]->Configurations->VideoEncoder->token;
                data.VideoEncoderConfigurationEncoding = ns1__GetProfilesResponse.Profiles[i]->Configurations->VideoEncoder->Encoding;
            }
        }
        ns1_onvif_Profiles.push_back(data);
    }
    return true;
}

bool OnvifApi::media_GetStreamUri(_ns1_onvif_GetStreamUri ns1_onvif_GetStreamUri, _ns1_onvif_GetStreamUriResponse& ns1_onvif_GetStreamUriResponse){
    Media2BindingProxy proxyMedia(this->soap);
    std::string endpoint = getEndpointMedia();
    proxyMedia.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _ns1__GetStreamUri ns1__GetStreamUri;
    ns1__GetStreamUri.Protocol = ns1_onvif_GetStreamUri.Protocol;
    ns1__GetStreamUri.ProfileToken = ns1_onvif_GetStreamUri.ProfileToken;
    _ns1__GetStreamUriResponse ns1__GetStreamUriResponse;
    if (proxyMedia.GetStreamUri(&ns1__GetStreamUri, ns1__GetStreamUriResponse))
    {
        report_error();
        return false;
    }
    ns1_onvif_GetStreamUriResponse.Uri = ns1__GetStreamUriResponse.Uri;
    return true;
}

bool OnvifApi::media_GetVideoEncoderConfigurations(_ns1_onvif_GetVideoEncoderConfiguration ns1_onvif_GetVideoEncoderConfiguration, std::vector <_ns1_onvif_VideoEncoderConfiguration> &ns1_onvif_VideoEncoderConfigurations){
    Media2BindingProxy proxyMedia(this->soap);
    std::string endpoint = getEndpointMedia();
    proxyMedia.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    ns1__GetConfiguration ns1__GetVideoEncoderConfigurations;
    if(ns1_onvif_GetVideoEncoderConfiguration.ProfileToken){
        ns1__GetVideoEncoderConfigurations.ProfileToken = new std::string(*ns1_onvif_GetVideoEncoderConfiguration.ProfileToken);
    }
    if(ns1_onvif_GetVideoEncoderConfiguration.ConfigurationToken){
        ns1__GetVideoEncoderConfigurations.ConfigurationToken = new std::string(*ns1_onvif_GetVideoEncoderConfiguration.ConfigurationToken);
    }
    _ns1__GetVideoEncoderConfigurationsResponse ns1__GetVideoEncoderConfigurationsResponse;
    if (proxyMedia.GetVideoEncoderConfigurations(&ns1__GetVideoEncoderConfigurations, ns1__GetVideoEncoderConfigurationsResponse))
    {
        report_error();
        return false;
    }
    for(long unsigned int i = 0; i < ns1__GetVideoEncoderConfigurationsResponse.Configurations.size(); i++){
        _ns1_onvif_VideoEncoderConfiguration data;
        data.token = ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->token;
        if(ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->Resolution){
            data.Resolution.Width = ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->Resolution->Width;
            data.Resolution.Height = ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->Resolution->Height;
        }
        data.Quality = ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->Quality;
        if(ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->RateControl){
            data.RateControl.FrameRateLimit = ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->RateControl->FrameRateLimit;
            // data.RateControl.ConstantBitRate = ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->RateControl->ConstantBitRate;
            data.RateControl.BitrateLimit = ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->RateControl->BitrateLimit;
        }
        data.Encoding = ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->Encoding;
        if(ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->Profile){
            data.Profile = *ns1__GetVideoEncoderConfigurationsResponse.Configurations[i]->Profile;
        }
        ns1_onvif_VideoEncoderConfigurations.push_back(data);
    }
    return true;
}


bool OnvifApi::media_SetVideoEncoderConfiguration(_ns1_onvif_VideoEncoderConfiguration ns1_onvif_VideoEncoderConfiguration){
    Media2BindingProxy proxyMedia(this->soap);
    std::string endpoint = getEndpointMedia();
    proxyMedia.soap_endpoint = endpoint.c_str();
    // set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    // _ns1__GetVideoEncoderConfiguration ns1__GetVideoEncoderConfiguration;
    // ns1__GetVideoEncoderConfiguration.ConfigurationToken = ns1_onvif_VideoEncoderConfiguration.token;
    // _ns1__GetVideoEncoderConfigurationResponse ns1__GetVideoEncoderConfigurationResponse;
    // if (proxyMedia.GetVideoEncoderConfiguration(&ns1__GetVideoEncoderConfiguration, ns1__GetVideoEncoderConfigurationResponse))
    // {
    //     report_error();
    //     return false;
    // }
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _ns1__SetVideoEncoderConfiguration ns1__SetVideoEncoderConfiguration;
    ns1__SetVideoEncoderConfiguration.Configuration = soap_new_tt__VideoEncoder2Configuration(this->soap);
    ns1__SetVideoEncoderConfiguration.Configuration->token = ns1_onvif_VideoEncoderConfiguration.token;
    ns1__SetVideoEncoderConfiguration.Configuration->Encoding = ns1_onvif_VideoEncoderConfiguration.Encoding;
    ns1__SetVideoEncoderConfiguration.Configuration->Resolution = soap_new_tt__VideoResolution2(this->soap);
    ns1__SetVideoEncoderConfiguration.Configuration->Resolution->Width = ns1_onvif_VideoEncoderConfiguration.Resolution.Width;
    ns1__SetVideoEncoderConfiguration.Configuration->Resolution->Height = ns1_onvif_VideoEncoderConfiguration.Resolution.Height;
    ns1__SetVideoEncoderConfiguration.Configuration->Quality = ns1_onvif_VideoEncoderConfiguration.Quality;
    ns1__SetVideoEncoderConfiguration.Configuration->RateControl = soap_new_tt__VideoRateControl2(this->soap);
    ns1__SetVideoEncoderConfiguration.Configuration->RateControl->FrameRateLimit = ns1_onvif_VideoEncoderConfiguration.RateControl.FrameRateLimit;
    ns1__SetVideoEncoderConfiguration.Configuration->RateControl->BitrateLimit = ns1_onvif_VideoEncoderConfiguration.RateControl.BitrateLimit;
    
    ns1__SetConfigurationResponse ns1__SetVideoEncoderConfigurationResponse;
    if (proxyMedia.SetVideoEncoderConfiguration(&ns1__SetVideoEncoderConfiguration, ns1__SetVideoEncoderConfigurationResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::imaging_GetImagingSettings(ClientInfo clientInfo, _timg__GetImagingSettings *timg__GetImagingSettings, _timg__GetImagingSettingsResponse &timg__GetImagingSettingsResponse){
    ImagingBindingProxy proxyImaging(this->soap);
    std::string endpoint = getEndpointImaging();
    proxyImaging.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyImaging.GetImagingSettings(timg__GetImagingSettings, timg__GetImagingSettingsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::imaging_GetMoveOptions(ClientInfo clientInfo, _timg__GetMoveOptions *timg__GetMoveOptions, _timg__GetMoveOptionsResponse &timg__GetMoveOptionsResponse){
    ImagingBindingProxy proxyImaging(this->soap);
    std::string endpoint = getEndpointImaging();
    proxyImaging.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyImaging.GetMoveOptions(timg__GetMoveOptions, timg__GetMoveOptionsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::imaging_GetOptions(ClientInfo clientInfo, _timg__GetOptions *timg__GetOptions, _timg__GetOptionsResponse &timg__GetOptionsResponse){
    ImagingBindingProxy proxyImaging(this->soap);
    std::string endpoint = getEndpointImaging();
    proxyImaging.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyImaging.GetOptions(timg__GetOptions, timg__GetOptionsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::imaging_GetStatus(ClientInfo clientInfo, _timg__GetStatus *timg__GetStatus, _timg__GetStatusResponse &timg__GetStatusResponse){
    ImagingBindingProxy proxyImaging(this->soap);
    std::string endpoint = getEndpointImaging();
    proxyImaging.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyImaging.GetStatus(timg__GetStatus, timg__GetStatusResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::imaging_Move(ClientInfo clientInfo, _timg__Move *timg__Move, _timg__MoveResponse &timg__MoveResponse){
    ImagingBindingProxy proxyImaging(this->soap);
    std::string endpoint = getEndpointImaging();
    proxyImaging.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyImaging.Move(timg__Move, timg__MoveResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::imaging_SetImagingSettings(ClientInfo clientInfo, _timg__SetImagingSettings *timg__SetImagingSettings, _timg__SetImagingSettingsResponse &timg__SetImagingSettingsResponse){
    ImagingBindingProxy proxyImaging(this->soap);
    std::string endpoint = getEndpointImaging();
    proxyImaging.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyImaging.SetImagingSettings(timg__SetImagingSettings, timg__SetImagingSettingsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::imaging_Stop(ClientInfo clientInfo, _timg__Stop *timg__Stop, _timg__StopResponse &timg__StopResponse){
    ImagingBindingProxy proxyImaging(this->soap);
    std::string endpoint = getEndpointImaging();
    proxyImaging.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyImaging.Stop(timg__Stop, timg__StopResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::imaging_GetServiceCapabilities(ClientInfo clientInfo, _timg__GetServiceCapabilities *timg__GetServiceCapabilities, _timg__GetServiceCapabilitiesResponse &timg__GetServiceCapabilitiesResponse){
    ImagingBindingProxy proxyImaging(this->soap);
    std::string endpoint = getEndpointImaging();
    proxyImaging.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyImaging.GetServiceCapabilities(timg__GetServiceCapabilities, timg__GetServiceCapabilitiesResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_AbsoluteMove(ClientInfo clientInfo, _tptz__AbsoluteMove *tptz__AbsoluteMove, _tptz__AbsoluteMoveResponse &tptz__AbsoluteMoveResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.AbsoluteMove(tptz__AbsoluteMove, tptz__AbsoluteMoveResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_ContinuousMove(ClientInfo clientInfo, _tptz__ContinuousMove *tptz__ContinuousMove, _tptz__ContinuousMoveResponse &tptz__ContinuousMoveResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.ContinuousMove(tptz__ContinuousMove, tptz__ContinuousMoveResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetConfiguration(ClientInfo clientInfo, _tptz__GetConfiguration *tptz__GetConfiguration, _tptz__GetConfigurationResponse &tptz__GetConfigurationResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetConfiguration(tptz__GetConfiguration, tptz__GetConfigurationResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetConfigurationOptions(ClientInfo clientInfo, _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, _tptz__GetConfigurationOptionsResponse &tptz__GetConfigurationOptionsResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetConfigurationOptions(tptz__GetConfigurationOptions, tptz__GetConfigurationOptionsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetConfigurations(ClientInfo clientInfo, _tptz__GetConfigurations *tptz__GetConfigurations, _tptz__GetConfigurationsResponse &tptz__GetConfigurationsResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetConfigurations(tptz__GetConfigurations, tptz__GetConfigurationsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetNode(ClientInfo clientInfo, _tptz__GetNode *tptz__GetNode, _tptz__GetNodeResponse &tptz__GetNodeResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetNode(tptz__GetNode, tptz__GetNodeResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetNodes(ClientInfo clientInfo, _tptz__GetNodes *tptz__GetNodes, _tptz__GetNodesResponse &tptz__GetNodesResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetNodes(tptz__GetNodes, tptz__GetNodesResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetPresets(ClientInfo clientInfo, _tptz__GetPresets *tptz__GetPresets, _tptz__GetPresetsResponse &tptz__GetPresetsResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetPresets(tptz__GetPresets, tptz__GetPresetsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetStatus(ClientInfo clientInfo, _tptz__GetStatus *tptz__GetStatus, _tptz__GetStatusResponse &tptz__GetStatusResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetStatus(tptz__GetStatus, tptz__GetStatusResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GotoHomePosition(ClientInfo clientInfo, _tptz__GotoHomePosition *tptz__GotoHomePosition, _tptz__GotoHomePositionResponse &tptz__GotoHomePositionResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GotoHomePosition(tptz__GotoHomePosition, tptz__GotoHomePositionResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GotoPreset(ClientInfo clientInfo, _tptz__GotoPreset *tptz__GotoPreset, _tptz__GotoPresetResponse &tptz__GotoPresetResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GotoPreset(tptz__GotoPreset, tptz__GotoPresetResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_RelativeMove(ClientInfo clientInfo, _tptz__RelativeMove *tptz__RelativeMove, _tptz__RelativeMoveResponse &tptz__RelativeMoveResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.RelativeMove(tptz__RelativeMove, tptz__RelativeMoveResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_RemovePreset(ClientInfo clientInfo, _tptz__RemovePreset *tptz__RemovePreset, _tptz__RemovePresetResponse &tptz__RemovePresetResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.RemovePreset(tptz__RemovePreset, tptz__RemovePresetResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_SetConfiguration(ClientInfo clientInfo, _tptz__SetConfiguration *tptz__SetConfiguration, _tptz__SetConfigurationResponse &tptz__SetConfigurationResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.SetConfiguration(tptz__SetConfiguration, tptz__SetConfigurationResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_SetHomePosition(ClientInfo clientInfo, _tptz__SetHomePosition *tptz__SetHomePosition, _tptz__SetHomePositionResponse &tptz__SetHomePositionResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.SetHomePosition(tptz__SetHomePosition, tptz__SetHomePositionResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_SetPreset(ClientInfo clientInfo, _tptz__SetPreset *tptz__SetPreset, _tptz__SetPresetResponse &tptz__SetPresetResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.SetPreset(tptz__SetPreset, tptz__SetPresetResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_Stop(ClientInfo clientInfo, _tptz__Stop *tptz__Stop, _tptz__StopResponse &tptz__StopResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.Stop(tptz__Stop, tptz__StopResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetPresetTour(ClientInfo clientInfo, _tptz__GetPresetTour *tptz__GetPresetTour, _tptz__GetPresetTourResponse &tptz__GetPresetTourResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetPresetTour(tptz__GetPresetTour, tptz__GetPresetTourResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetPresetTours(ClientInfo clientInfo, _tptz__GetPresetTours *tptz__GetPresetTours, _tptz__GetPresetToursResponse &tptz__GetPresetToursResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetPresetTours(tptz__GetPresetTours, tptz__GetPresetToursResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetPresetTourOptions(ClientInfo clientInfo, _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, _tptz__GetPresetTourOptionsResponse &tptz__GetPresetTourOptionsResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetPresetTourOptions(tptz__GetPresetTourOptions, tptz__GetPresetTourOptionsResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_CreatePresetTour(ClientInfo clientInfo, _tptz__CreatePresetTour *tptz__CreatePresetTour, _tptz__CreatePresetTourResponse &tptz__CreatePresetTourResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.CreatePresetTour(tptz__CreatePresetTour, tptz__CreatePresetTourResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_ModifyPresetTour(ClientInfo clientInfo, _tptz__ModifyPresetTour *tptz__ModifyPresetTour, _tptz__ModifyPresetTourResponse &tptz__ModifyPresetTourResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.ModifyPresetTour(tptz__ModifyPresetTour, tptz__ModifyPresetTourResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_OperatePresetTour(ClientInfo clientInfo, _tptz__OperatePresetTour *tptz__OperatePresetTour, _tptz__OperatePresetTourResponse &tptz__OperatePresetTourResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.OperatePresetTour(tptz__OperatePresetTour, tptz__OperatePresetTourResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_RemovePresetTour(ClientInfo clientInfo, _tptz__RemovePresetTour *tptz__RemovePresetTour, _tptz__RemovePresetTourResponse &tptz__RemovePresetTourResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.RemovePresetTour(tptz__RemovePresetTour, tptz__RemovePresetTourResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::ptz_GetServiceCapabilities(ClientInfo clientInfo, _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, _tptz__GetServiceCapabilitiesResponse &tptz__GetServiceCapabilitiesResponse){
    PTZBindingProxy proxyPTZ(this->soap);
    std::string endpoint = getEndpointPtz();
    proxyPTZ.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyPTZ.GetServiceCapabilities(tptz__GetServiceCapabilities, tptz__GetServiceCapabilitiesResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::tev_Renew(std::string Adress, std::string TerminationTime){
    PullPointSubscriptionBindingProxy proxyEvent(this->soap);
    std::string endpoint = Adress;
    proxyEvent.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _wsnt__Renew wsnt__Renew;
    wsnt__Renew.TerminationTime = new std::string(TerminationTime);
    _wsnt__RenewResponse wsnt__RenewResponse;
    if (proxyEvent.Renew(&wsnt__Renew, wsnt__RenewResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::tev_SetSynchronizationPoint(ClientInfo clientInfo, _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, _tev__SetSynchronizationPointResponse &tev__SetSynchronizationPointResponse){
    PullPointSubscriptionBindingProxy proxyEvent(this->soap);
    std::string endpoint = getEndpointEvent();
    proxyEvent.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    if (proxyEvent.SetSynchronizationPoint(tev__SetSynchronizationPoint, tev__SetSynchronizationPointResponse))
    {
        report_error();
        return false;
    }
    return true;
}

bool OnvifApi::tev_CreatePullPointSubscription(std::string InitialTerminationTime, std::string& Address){
    PullPointSubscriptionBindingProxy proxyEvent(this->soap);
    std::string endpoint = getEndpointEvent();
    // std::cout << "endpoint: " << endpoint << std::endl;
    proxyEvent.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tev__CreatePullPointSubscription tev__CreatePullPointSubscription;
    if(InitialTerminationTime != ""){
        tev__CreatePullPointSubscription.InitialTerminationTime = new std::string(InitialTerminationTime);
    }
    _tev__CreatePullPointSubscriptionResponse tev__CreatePullPointSubscriptionResponse;
    if (proxyEvent.CreatePullPointSubscription(&tev__CreatePullPointSubscription, tev__CreatePullPointSubscriptionResponse))
    {
        report_error();
        return false;
    }
    Address = tev__CreatePullPointSubscriptionResponse.SubscriptionReference.Address;
    return true;
}

bool OnvifApi::tev_PullMessages(std::string Address, _tev_onvif_PullMessages tev_onvif_PullMessages, _tev_onvif_PullMessagesResponse& tev_onvif_PullMessagesResponse){
    struct soap *soap2 = soap_copy(this->soap);
    this->soap->header->wsa5__Action = (char*)"http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesRequest";
	// this->soap->header->wsa5__To = (char*)"http://www.w3.org/2005/08/addressing/anonymous";
    
    PullPointSubscriptionBindingProxy proxyEvent(this->soap);
    std::string endpoint = Address;
    // std::string endpoint = getEndpointEvent();
    // std::cout << "endpoint: " << endpoint << std::endl;
    proxyEvent.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tev__PullMessages tev__PullMessages;
    tev__PullMessages.Timeout = tev_onvif_PullMessages.Timeout;
    tev__PullMessages.MessageLimit = tev_onvif_PullMessages.MessageLimit;
    _tev__PullMessagesResponse tev__PullMessagesResponse;
    if (proxyEvent.PullMessages(&tev__PullMessages, tev__PullMessagesResponse))
    {
        report_error();
        std::cout << "PullMessages: false" << std::endl;
        // this->soap = soap_copy(soap2);
        return false;
    }
    tev_onvif_PullMessagesResponse.CurrentTime = tev__PullMessagesResponse.CurrentTime;
    tev_onvif_PullMessagesResponse.TerminationTime = tev__PullMessagesResponse.TerminationTime;
    for(long unsigned int i = 0; i < tev__PullMessagesResponse.wsnt__NotificationMessage.size(); i++){
        _tev_onvif_NotificationMessage data;
        data.Topic = tev__PullMessagesResponse.wsnt__NotificationMessage[i]->Topic->__any;
        data.Message = tev__PullMessagesResponse.wsnt__NotificationMessage[i]->Message.__any;
        tev_onvif_PullMessagesResponse.NotificationMessage.push_back(data);
    }
    // this->soap = soap_copy(soap2);
    return true;
}

bool OnvifApi::tev_GetEventProperties(_tev_onvif_GetEventPropertiesResponse &tev_onvif_GetEventPropertiesResponse){
    PullPointSubscriptionBindingProxy proxyEvent(this->soap);
    std::string endpoint = getEndpointEvent();
    proxyEvent.soap_endpoint = endpoint.c_str();
    set_credentials(this->clientInfo.username, this->clientInfo.password, this->clientInfo.isDigestLogin);
    _tev__GetEventProperties tev__GetEventProperties;
    _tev__GetEventPropertiesResponse tev__GetEventPropertiesResponse;
    if (proxyEvent.GetEventProperties(&tev__GetEventProperties, tev__GetEventPropertiesResponse))
    {
        report_error();
        return false;
    }
    for(long unsigned int i = 0; i < tev__GetEventPropertiesResponse.wstop__TopicSet->__any.size(); i++){
        tev_onvif_GetEventPropertiesResponse.TopicSet.push_back(tev__GetEventPropertiesResponse.wstop__TopicSet->__any[i]);
    }
    return true;
}



/******************************************************************************\
 *
 *	WS-Discovery event handlers must be defined, even when not used
 *
\******************************************************************************/

void wsdd_event_Hello(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int MetadataVersion)
{ 
    std::cout << __func__ << " - " << __LINE__ << std::endl;
}

void wsdd_event_Bye(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, const char *EndpointReference, const char *Types, const char *Scopes, const char *MatchBy, const char *XAddrs, unsigned int *MetadataVersion)
{ 
    std::cout << __func__ << " - " << __LINE__ << std::endl;
}

soap_wsdd_mode wsdd_event_Probe(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *Types, const char *Scopes, const char *MatchBy, struct wsdd__ProbeMatchesType *ProbeMatches)
{
    std::cout << __func__ << " - " << __LINE__ << std::endl;
  	return SOAP_WSDD_ADHOC;
}

void wsdd_event_ProbeMatches(struct soap *soap, unsigned int InstanceId, const char *SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ProbeMatchesType *ProbeMatches)
{
    std::cout << __func__ << " - " << __LINE__ << std::endl;
 }

soap_wsdd_mode wsdd_event_Resolve(struct soap *soap, const char *MessageID, const char *ReplyTo, const char *EndpointReference, struct wsdd__ResolveMatchType *match)
{
    std::cout << __func__ << " - " << __LINE__ << std::endl;
  	return SOAP_WSDD_ADHOC;
}

void wsdd_event_ResolveMatches(struct soap *soap, unsigned int InstanceId, const char * SequenceId, unsigned int MessageNumber, const char *MessageID, const char *RelatesTo, struct wsdd__ResolveMatchType *match)
{
    std::cout << __func__ << " - " << __LINE__ << std::endl;
 }

int SOAP_ENV__Fault(struct soap *soap, char *faultcode, char *faultstring, char *faultactor, struct SOAP_ENV__Detail *detail, struct SOAP_ENV__Code *SOAP_ENV__Code, struct SOAP_ENV__Reason *SOAP_ENV__Reason, char *SOAP_ENV__Node, char *SOAP_ENV__Role, struct SOAP_ENV__Detail *SOAP_ENV__Detail)
{
    std::cout << __func__ << " - " << __LINE__ << std::endl;
	// populate the fault struct from the operation arguments to print it
	soap_fault(soap);
	// SOAP 1.1
	soap->fault->faultcode = faultcode;
	soap->fault->faultstring = faultstring;
	soap->fault->faultactor = faultactor;
	soap->fault->detail = detail;
	// SOAP 1.2
	soap->fault->SOAP_ENV__Code = SOAP_ENV__Code;
	soap->fault->SOAP_ENV__Reason = SOAP_ENV__Reason;
	soap->fault->SOAP_ENV__Node = SOAP_ENV__Node;
	soap->fault->SOAP_ENV__Role = SOAP_ENV__Role;
	soap->fault->SOAP_ENV__Detail = SOAP_ENV__Detail;
	// set error
	soap->error = SOAP_FAULT;
	// handle or display the fault here with soap_stream_fault(soap, std::cerr);
	// return HTTP 202 Accepted
	return soap_send_empty_response(soap, SOAP_OK);
}