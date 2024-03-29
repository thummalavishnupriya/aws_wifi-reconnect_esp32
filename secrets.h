#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "esp32_TVOC"                         //change this
 
const char WIFI_SSID[] = "inGO_basement";               //change this
const char WIFI_PASSWORD[] = "Ingo12345";           //change this
const char AWS_IOT_ENDPOINT[] = "a3kxlwic1muc4u-ats.iot.ap-southeast-2.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUPjVb+yFs9gY8mKmEYc1ToTXccrYwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MDIyMTA1NTYx
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALsyI5/d8JQ9UFR+zIX7
DU1jjcqvQI3yGwOf8QircMuCXodAkPD5clD/Ce5bzcQdKq/88tA5apfA2DeZZyfL
Vjp2QfeKAHw2P52gbH6Z2FvzXZGQRWqUpQL6HhhHoW31iWfgvDl4tGX0gX9oEkHc
Q/ATqw265VJ2xT/8fblXty9VG1BYgsaqOybtIPm0vNJVyho/yweMH3SwDEbQUPq+
XcGRjkaHnFgOu0eNwKsRB5DEH/l83Dg9gVlL/R0kgk0WfyiBoXifHs9ZKzTrR8F+
ZzMU556umtXuaG/0iwrYOhey1wMOzKTR9u6BrUFXNLZi7ehEFpFW7KRQKIXPRRjR
g1UCAwEAAaNgMF4wHwYDVR0jBBgwFoAUoNEcMwSED8IBt3PnavYZtYj9ERUwHQYD
VR0OBBYEFG0eeTCr1jqHGs886UB/gm3BfcWGMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAFOz3t46LWtyLOehfadatlZLHy
cdVyQ/D6s4WmWt2W7tEzLj0Wot4Wrht//JVV5d3z4AQQmnipo2cloI4XI6pQWeQ/
vdmO924FLLgMWSNLgCu4h9PR0rbu0h4YjQzqwYn14AsX4lylXz29h2I+W542gdqD
dW7s4GoJs19IJLw2ICh/S7iN6F3Unx7sFt+Tre2uy3s+vtLWv4rsE3MLf7VAIecv
34603eGXVCWE5jr/tj6RrmAl5C+qPxdzSM0tn7yLv32Fmb0AEcRu8uDAld4l1v2s
QFnqGoH4K9T/ISs5ViLdovOmr1ffgtPy2P7j61b5gwAOV4eo+CQBaMgnydcP 
-----END CERTIFICATE-----
 
 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAuzIjn93wlD1QVH7MhfsNTWONyq9AjfIbA5/xCKtwy4Jeh0CQ
8PlyUP8J7lvNxB0qr/zy0Dlql8DYN5lnJ8tWOnZB94oAfDY/naBsfpnYW/NdkZBF
apSlAvoeGEehbfWJZ+C8OXi0ZfSBf2gSQdxD8BOrDbrlUnbFP/x9uVe3L1UbUFiC
xqo7Ju0g+bS80lXKGj/LB4wfdLAMRtBQ+r5dwZGORoecWA67R43AqxEHkMQf+Xzc
OD2BWUv9HSSCTRZ/KIGheJ8ez1krNOtHwX5nMxTnnq6a1e5ob/SLCtg6F7LXAw7M
pNH27oGtQVc0tmLt6EQWkVbspFAohc9FGNGDVQIDAQABAoIBAGL0ekiENizmzJQe
A1fAAqQqQEYopmNi3EzqO6xtlE7AN+AfjX8VHg0bBNDvzrIhCPPHL0RLVcGqu6GI
gjNTFd4JrY2Bcgg3VSGqMDAse2cAGzJbpZZVxXTxav9Cvl82MNCsyFNJL3PF8L68
8QnHulIq032Kkrco+dR9UKitabSmzO5xsmA3lUMbs07Hycpu71wkpbWOoMh8GkJv
kRzDKkggbsIPfEyXOrV4hDqawa2UDze6gKXYAuY/UIguCvm1Kd3twQ1FRcDCpOC9
mkmymZgcskdOHYHpfMdqn0tI3p5Mt9xXjhAjXBA32I1LcDTz5zrIQKO6ZTynNLsc
Bglz1vECgYEA5mOwFIoavdK8y+uGHdPwkVL7O0lS8zNCL1SbpMYW1jWdhc1seMAT
fQnSjlE0U83zJS6UAYkEwv7e+KUpd43nUjXI5F901351yI4M+1GKXK6z6U0LDyPd
0ojPb6LV2paOG/1IhreT2+I6VCKIstuN6SXS1k5DULWga6wrH9lrR1cCgYEA0AFF
FLEfN9kh8wYAVFB41byvoruAY80c2r12Ds7a95Nw3ScJecEKaVTKwtYDJ6ADk6Mz
ZW0YlEabE9ercvgrsN8J+JNmI+uXO34H3S/djYbYRWA1OuJmMdBFip4/ITt7DYpB
v3Dpeu/BkFr8hIMakRR/pmJiq0em8uTtO8KA+zMCgYAGqWVDAsdJK9nHr7S+Eg3H
tfWLvlAGtjm4sqKZfhzU7tHyN4FmcG9f0zPiZ5iyv6bDMXiYHsUjekkqOWRcj57b
srBvYg831/FB7lGTxsTQb0DGgzLJqkjUuJ3+5V93pZjQWF+Lfa1p0z3KhwEa4txU
nIJfLf4mTEoqMt1AW4INmwKBgC/oyveMIGulqIsoS7Uf3+UjE7VAyL60BLxhPSsF
/CtgG3PkxOr1SqeeLyGXV5yt34UG5xSqfynP5XI5j7Tocm/Hv60wjish3XCHg9ov
ZzFs6K16DmiksvMwTIQ0FMhbwekJekFVCYIWbRgJzPXcmKbn+MrjCGT5GPwDPJ0y
HLE/AoGBAM8w7+D0eaY8C0/AfDyr+J8RrldibjLe/ZI/HBLyLypW959LRWJHTesf
IvwUu9/P/HMAQ9qbpkTJ1Cs2fQLe7jaXzOjlOJT1tdsVCSaBSfOs7hhOfQZa/b/Y
Td/ARbdFgV4crwqsGqpmG5A4DJiqNHe1txogjOFeZuyOmPPnM0Uq
-----END RSA PRIVATE KEY-----
 
 
)KEY";
