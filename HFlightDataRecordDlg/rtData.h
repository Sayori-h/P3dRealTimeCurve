#pragma once
// Copyright (c) 2010-2018 Lockheed Martin Corporation. All rights reserved.
// Use of this file is bound by the PREPAR3D® SOFTWARE DEVELOPER KIT END USER LICENSE AGREEMENT 
//------------------------------------------------------------------------------
//
//  SimConnect Data Harvester Sample
//  
//  Description:
//              Requests data on the user object and outputs it to a CSV file.
//              If the sample application is ran from within the SimConnect
//              Samples solution, the CSV file will be created relative to the
//              DataHarvester project file. If the sample application is executed
//              directly, the CSV file will be created relative to the application.
//              Start and stop data harvesting from the Prepar3D Add-ons menu item.
//------------------------------------------------------------------------------

#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <timeapi.h>
#include <time.h>
#pragma comment(lib,"Winmm.lib")

#include "SimConnect.h"
int     g_bQuit = 0;
HANDLE  g_hSimConnect = NULL;
bool    g_bIsHarvesting = false;
void setColour(int x) {
	HANDLE h = GetStdHandle(-11);
	SetConsoleTextAttribute(h, x);
}

#pragma warning(disable:4996) 
///////////////////////////////////////////////////

#define USE_NEW_TIME 1
#define WM_USER_MSG_HDL    (WM_USER + 100)
#define OUTPUT_WAY 1

double dev_windX, dev_windY, dev_windZ;

// 时:分:秒
void GetTimeStr(char* pBuffer, int nBuffertLength, double dTime)
{
	int nDaySec = (int)dTime;
	char szTemp[128] = { 0 };//预先分配源字符串缓冲区

	int nTime = nDaySec / 3600;
	int nMin = (nDaySec - nTime * 3600) / 60;
	int nSec = nDaySec % 60;

	sprintf_s(szTemp, 128, "%d:%d:%d,", nTime, nMin, nSec);
	strcat_s(pBuffer, nBuffertLength, szTemp);
}
//年/月/日
void GetDateStr(char* pBuffer, int nBuffertLength, int nYear, int nMonth, int nDay)
{
	char szTemp[128] = { 0 };
	sprintf_s(szTemp, 128, "%d/%d/%d,", nYear, nMonth, nDay);
	strcat_s(pBuffer, nBuffertLength, szTemp);
}

//windX
void GetWindX(char* pBuffer, int nBuffertLength, double dPitch, double dBank, double dHeading, double dAlpha,
	double dBeta, double dAirspeed, double dwvelocityz)
{
	char szTemp[128] = { 0 };
	dPitch *= (-1);
	double dwx = dwvelocityz - dAirspeed * (cos(dAlpha) * cos(dPitch) * cos(dHeading) * cos(dBeta) + \
		sin(dBeta) * sin(dPitch) * sin(dBank) * cos(dHeading) - sin(dBeta) * cos(dBank) * sin(dHeading) + \
		sin(dHeading) * sin(dBank) * sin(dAlpha) * cos(dBeta) + cos(dHeading) * sin(dPitch) * cos(dBank) * sin(dAlpha) * cos(dBeta));
	dev_windX = dwx;
	sprintf_s(szTemp, 128, "%.2lf,", dwx);
	strcat_s(pBuffer, nBuffertLength, szTemp);
}

//windY
void GetWindY(char* pBuffer, int nBuffertLength, double dPitch, double dBank, double dHeading, double dAlpha,
	double dBeta, double dAirspeed, double dwvelocityx)
{
	char szTemp[128] = { 0 };
	dPitch *= (-1);
	double dwy = dwvelocityx - dAirspeed * (cos(dAlpha) * cos(dBeta) * sin(dHeading) * cos(dPitch) + \
		sin(dBeta) * sin(dPitch) * sin(dBank) * sin(dHeading) + sin(dBeta) * cos(dBank) * cos(dHeading) - \
		cos(dHeading) * sin(dAlpha) * cos(dBeta) * sin(dBank) + cos(dBank) * sin(dPitch) * sin(dHeading) * sin(dAlpha) * cos(dBeta));
	dev_windY = dwy;
	sprintf_s(szTemp, 128, "%.2lf,", dwy);
	strcat_s(pBuffer, nBuffertLength, szTemp);
}

//windZ
void GetWindZ(char* pBuffer, int nBuffertLength, double dPitch, double dBank, double dHeading, double dAlpha,
	double dBeta, double dAirspeed, double dwvelocityy)
{
	char szTemp[128] = { 0 };
	dPitch *= (-1);
	double dwz = dwvelocityy - dAirspeed * (sin(dAlpha) * cos(dBeta) * cos(dPitch) * cos(dBank) - \
		cos(dAlpha) * sin(dPitch) * cos(dBeta) + sin(dBeta) * sin(dBank) * cos(dPitch));
	dev_windZ = dwz;
	sprintf_s(szTemp, 128, "%.2lf,", dwz);
	strcat_s(pBuffer, nBuffertLength, szTemp);
}

int nCount1Sec = 100;
unsigned int nTimeOutMS = 1000 / nCount1Sec;
time_t tCurSec = 0;
unsigned int nLastMS = 0;

/////////////////////////////////////////////////////////


static const char* TITLE_STRING = "Data Harvester";
static const char* START_STRING = "Start Harvest (SimConnect)";
static const char* STOP_STRING = "Stop Harvest (SimConnect)";

FILE* g_pFile = NULL;
static const char* OUTPUT_FILE_NAME = "DataHarvester.csv";


enum GROUP_ID
{
	GROUP_ID_MENU,
};

enum EVENT_ID
{
	EVENT_ID_START_HARVEST,
	EVENT_ID_STOP_HARVEST,
};

enum DEFINITION_ID
{
	DEFINITION_ID_USER_OBJECT,
};

enum REQUEST_ID
{
	REQUEST_ID_USER_OBJECT_DATA,
};

struct SendData
{
	int pitch_v;
	int pitch_acc;
	int roll_v;
	int roll_acc;
};

// This struct must align with the sizes and types defined in the table below.
struct ObjectData
{
	char    szTitle[256];
	double  dAbsoluteTime;
	double  dTime;
	double  dSimTime;
	int     uDayOfTheYear;
	int     uYear;
	int     uMonthOfTheYear;
	int     uDayOfTheMonth;
	double  dLatitude;
	double  dLongitude;
	double  dBank;
	double  dHeading;
	double  dAlpha;
	double  dBeta;
	double  dAirspeed;
	double  dHeight;
	double  dPitch;
	double  dElevator_def;
	double  dRudder_def;
	double  dAileron_left;
	double  dAileron_right;
	double  dWindDirection;
	double  dWindVelocity;
	double  dWindX;
	double  dWindY;
	double  dWindZ;
	double  dVerSpeed;
	double  dn1;
	double  dn2;
	double  dwvelocityz;
	double  dwvelocityx;
	double  dwvelocityy;
	double  dR_VelocityX;
	double  dR_VelocityY;
	double  dR_AccelerationX;
	double  dR_AccelerationY;
};

struct PropertyDefinition
{
	const char* pszName;
	const char* pszUnits;
	SIMCONNECT_DATATYPE eDataType;
};

// This table must align with the sizes and types defined in the struct above.
const PropertyDefinition g_aVariables[] =
{
	{ "TITLE",                             NULL,                   SIMCONNECT_DATATYPE_STRING256   },
	{ "ABSOLUTE TIME",                     "Seconds",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "ZULU TIME",                         "Seconds",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "SIM TIME",                          "Seconds",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "ZULU DAY OF YEAR",                  "Number",               SIMCONNECT_DATATYPE_INT32       },
	{ "ZULU YEAR",                         "Number",               SIMCONNECT_DATATYPE_INT32       },
	{ "ZULU MONTH OF YEAR",                "Number",               SIMCONNECT_DATATYPE_INT32       },
	{ "ZULU DAY OF MONTH",                 "Number",               SIMCONNECT_DATATYPE_INT32       },
	{ "PLANE LATITUDE",                    "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "PLANE LONGITUDE",                   "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "PLANE BANK DEGREES",                "Radians",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "PLANE HEADING DEGREES TRUE",        "Radians",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "INCIDENCE ALPHA",                   "Radians",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "INCIDENCE BETA",					   "Radians",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "AIRSPEED TRUE",					   "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64     },
	{ "PLANE ALTITUDE",					   "Feet",                 SIMCONNECT_DATATYPE_FLOAT64     },
	{ "PLANE PITCH DEGREES",               "Radians",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "ELEVATOR DEFLECTION",			   "Radians",			   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "RUDDER DEFLECTION",				   "Radians",			   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "AILERON LEFT DEFLECTION",           "Radians",			   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "AILERON RIGHT DEFLECTION",          "Radians",			   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "AMBIENT WIND DIRECTION",            "Degrees",              SIMCONNECT_DATATYPE_FLOAT64     },
	{ "AMBIENT WIND VELOCITY",             "Knots",                SIMCONNECT_DATATYPE_FLOAT64     },
	{ "AMBIENT WIND X",                    "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64     },
	{ "AMBIENT WIND Y",                    "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64     },
	{ "AMBIENT WIND Z",                    "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64     },
	{ "VERTICAL SPEED",                    "Feet per second",      SIMCONNECT_DATATYPE_FLOAT64     },
	{ "TURB ENG N1:1",                     "Percent",			   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "TURB ENG N1:2",                     "Percent",			   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "VELOCITY WORLD Z",                  "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64	   },
	{ "VELOCITY WORLD X",                  "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64	   },
	{ "VELOCITY WORLD Y",                  "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64	   },
	{ "VELOCITY WORLD X",                  "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64	   },
	{ "VELOCITY WORLD Y",                  "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64	   },
	{ "VELOCITY WORLD Z",                  "Meters per second",    SIMCONNECT_DATATYPE_FLOAT64	   },
	{ "ROTATION VELOCITY BODY X",          "Radians per second",   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "ROTATION VELOCITY BODY Y",          "Radians per second",   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "ROTATION ACCELERATION BODY X",      "Radians per second",   SIMCONNECT_DATATYPE_FLOAT64     },
	{ "ROTATION ACCELERATION BODY Y",      "Radians per second",   SIMCONNECT_DATATYPE_FLOAT64     },
};

double winddata[10];
SOCKET socket1;
struct sockaddr_in server;
int len = sizeof(server);

void PrintHeader()
{
	char szBuffer[2048] = { 0 };

	for (unsigned int i = 0; i < ARRAYSIZE(g_aVariables); ++i)
	{
		const PropertyDefinition& prop = g_aVariables[i];


#if USE_NEW_TIME
		if (1 == i) continue;
		if (5 == i) continue;
		if (6 == i) continue;
#else
#endif

		if (2 == i) { strcat_s(szBuffer, sizeof(szBuffer), "ZULU UTC TIME"); }
		else if (7 == i) { strcat_s(szBuffer, sizeof(szBuffer), "ZULU DATE"); }
		else if (29 == i) { strcat_s(szBuffer, sizeof(szBuffer), "DEFINED WIND X"); }
		else if (30 == i) { strcat_s(szBuffer, sizeof(szBuffer), "DEFINED WIND Y"); }
		else if (31 == i) { strcat_s(szBuffer, sizeof(szBuffer), "DEFINED WIND Z"); }
		else { strcat_s(szBuffer, sizeof(szBuffer), prop.pszName); }


		if (prop.pszUnits)
		{
			char szUnits[64] = { 0 };
			StringCchPrintfA(szUnits, sizeof(szUnits), " (%s)", prop.pszUnits);
			strcat_s(szBuffer, sizeof(szBuffer), szUnits);
		}

		if (i != ARRAYSIZE(g_aVariables) - 1)
		{
			strcat_s(szBuffer, sizeof(szBuffer), ",");
		}
	}

	strcat_s(szBuffer, sizeof(szBuffer), "\n");

	fprintf_s(g_pFile, szBuffer);
}

void PrintString(char* pszBuffer, unsigned int cbBuffer, const char* pszValue, bool bIsLast = false)
{
	if (pszBuffer != NULL && cbBuffer > 0)
	{
		char szTemp[256] = { 0 };
		if (!bIsLast)
		{
			if (pszValue)
			{
				StringCchPrintfA(szTemp, sizeof(szTemp), "%s,", pszValue);
				strcat_s(pszBuffer, cbBuffer, szTemp);
			}
			else
			{
				StringCchPrintfA(szTemp, sizeof(szTemp), ",");
				strcat_s(pszBuffer, cbBuffer, szTemp);
			}
		}
		else
		{
			if (pszValue)
			{
				StringCchPrintfA(szTemp, sizeof(szTemp), "%s\n", pszValue);
				strcat_s(pszBuffer, cbBuffer, szTemp);
			}
			else
			{
				StringCchPrintfA(szTemp, sizeof(szTemp), "\n");
				strcat_s(pszBuffer, cbBuffer, szTemp);
			}
		}
	}
}

void PrintInt(char* pszBuffer, unsigned int cbBuffer, int iValue, bool bIsLast = false)
{
	if (pszBuffer != NULL && cbBuffer > 0)
	{
		char szTemp[128] = { 0 };
		if (!bIsLast)
		{
			StringCchPrintfA(szTemp, sizeof(szTemp), "%d,", iValue);
			strcat_s(pszBuffer, cbBuffer, szTemp);
		}
		else
		{
			StringCchPrintfA(szTemp, sizeof(szTemp), "%d\n", iValue);
			strcat_s(pszBuffer, cbBuffer, szTemp);
		}
	}
}

void PrintDouble(char* pszBuffer, unsigned int cbBuffer, double dValue)
{
	bool bIsLast = false;
	if (pszBuffer != NULL && cbBuffer > 0)
	{
		char szTemp[128] = { 0 };
		if (!bIsLast)
		{
			StringCchPrintfA(szTemp, sizeof(szTemp), "%.20lf,", dValue);
			strcat_s(pszBuffer, cbBuffer, szTemp);
		}
		else
		{
			StringCchPrintfA(szTemp, sizeof(szTemp), "%.20lf\n", dValue);
			strcat_s(pszBuffer, cbBuffer, szTemp);
		}
	}
}

void CALLBACK MyDispatchProcRD(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	HRESULT hr;
	SendData sd;
	
	HWND hWnd;
	if (!(hWnd = ::FindWindow(NULL, _T("HFlightDataRecord"))))
		AfxMessageBox(_T("窗口标题不一致!"));

	double data[4];

	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_EVENT:
	{
		SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;

		switch (evt->uEventID)
		{
		case EVENT_ID_START_HARVEST:
		{
			// Verify the socket has been opened. Notify the user otherwise.

			//InitWinsock();
			WSADATA wsaData;
			//int iErrorCode;
			if (WSAStartup(MAKEWORD(2, 1), &wsaData)) //调用Windows Sockets DLL
			{
				printf("Winsock无法初始化!\n");
				WSACleanup();
				return;
			}

			printf("服务器开始创建SOCKET。\n");

			socket1 = socket(AF_INET, SOCK_DGRAM, 0);
			server.sin_family = AF_INET;
			server.sin_port = htons(4096); ///server的监听端口
			//server.sin_addr.s_addr = inet_addr("192.168.0.10"); ///硬件设备的地址
			server.sin_addr.s_addr = inet_addr("127.0.0.1"); ///server的地址

			sd = { 1,1000,1,1000 };
			g_bIsHarvesting = true;

			// Verify the file has been opened. Notify the user otherwise.
			printf("\nAttempting to open CSV file...");
			if (g_pFile)
			{
				fclose(g_pFile);
				g_pFile = NULL;
			}

			char szFileName[1024];
			time_t t = time(NULL);
			struct tm* pTM = localtime(&t);

			sprintf_s(szFileName, 1024, "%04d_%02d_%02d_%02d_%02d_%02drecord.csv", pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday, pTM->tm_hour, pTM->tm_min, pTM->tm_sec);

			fopen_s(&g_pFile, szFileName, "w+");
			if (!g_pFile)
			{
				printf("Failed to open \"%s\". Please ensure the file has the correct permissions and is not being used by another application.", szFileName);
				break;
			}
			printf("\nFile successfully opened!");
			printf("\nStarting data harvest...");

			// Print variable titles into the CSV.
			PrintHeader();

			// Per frame data request on the user object.
			hr = SimConnect_RequestDataOnSimObject(g_hSimConnect, REQUEST_ID_USER_OBJECT_DATA, DEFINITION_ID_USER_OBJECT, SIMCONNECT_OBJECT_ID_USER, /*SIMCONNECT_PERIOD_VISUAL_FRAME*/SIMCONNECT_PERIOD_SECOND);

			// Update menu items.
			hr = SimConnect_MenuDeleteItem(g_hSimConnect, EVENT_ID_START_HARVEST);
			hr = SimConnect_MenuAddItem(g_hSimConnect, STOP_STRING, EVENT_ID_STOP_HARVEST, 0);

			break;
		}
		case EVENT_ID_STOP_HARVEST:
		{
			// Cancel the data request on the user object.
			hr = SimConnect_RequestDataOnSimObject(g_hSimConnect, REQUEST_ID_USER_OBJECT_DATA, DEFINITION_ID_USER_OBJECT, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_NEVER);

			// Update menu items.
			hr = SimConnect_MenuDeleteItem(g_hSimConnect, EVENT_ID_STOP_HARVEST);
			hr = SimConnect_MenuAddItem(g_hSimConnect, START_STRING, EVENT_ID_START_HARVEST, 0);

			sd = { 1,1000,1,1000 };

			char* buffer = new char[sizeof(SendData)];

			memcpy(buffer, &sd, sizeof(SendData));


			if (sendto(socket1, buffer, sizeof(SendData), 0, (struct sockaddr*)&server, len) != SOCKET_ERROR)
			{

				printf("回中..\n");

			}
			// Close the Socket.
			closesocket(socket1);

			printf("\nStopping data send.");

			// Close the file.
			if (g_pFile)
			{
				fclose(g_pFile);
				g_pFile = NULL;
			}

			printf("\nStopping data harvest.");

			g_bIsHarvesting = false;

			break;
		}
		default:
		{
			break;
		}
		}

		break; // SIMCONNECT_RECV_ID_EVENT
	}

	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
	{
		SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;

		switch (pObjData->dwRequestID)
		{
		case REQUEST_ID_USER_OBJECT_DATA:
		{
			DWORD dwObjectID = pObjData->dwObjectID;

			ObjectData* pUserData = (ObjectData*)&pObjData->dwData;

			// Console print a portion of the data.

			if (SUCCEEDED(StringCbLengthA(&pUserData->szTitle[0], sizeof(pUserData->szTitle), NULL))) // security check
			{
				winddata[0] = pUserData->dPitch;
				winddata[1] = pUserData->dBank;
				winddata[2] = pUserData->dHeading;
				winddata[3] = pUserData->dAlpha;
				winddata[4] = pUserData->dBeta;
				winddata[5] = pUserData->dAirspeed;
				winddata[6] = pUserData->dwvelocityz;
				winddata[7] = pUserData->dwvelocityx;
				winddata[8] = pUserData->dwvelocityy;
#if OUTPUT_WAY 
				setColour(2);
				printf("\n %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
					pUserData->dwvelocityz,
					pUserData->dwvelocityx,
					pUserData->dwvelocityy,
					pUserData->dWindX,
					pUserData->dWindY,
					pUserData->dWindZ,
					dev_windX,
					dev_windY,
					dev_windZ
				);
#else
#endif
			}
			
			sd.pitch_v = pUserData->dR_VelocityX * 30.0f / 3.1415926f * 100;
			sd.pitch_acc = pUserData->dR_AccelerationX * 2 * 3.1415926f * 100;
			sd.roll_v = pUserData->dR_VelocityY * 30.0f / 3.1415926f * 100;
			sd.roll_acc = pUserData->dR_AccelerationY * 2 * 3.1415926f * 100;

			char* buffer = new char[sizeof(SendData)];

			memcpy(buffer, &sd, sizeof(SendData));

			if (socket1 != NULL && sendto(socket1, buffer, sizeof(SendData), 0, (struct sockaddr*)&server, len) != SOCKET_ERROR)
			{
				printf("sending..\n");
			}
			delete []buffer;

			
			data[0] = pUserData->dSimTime;
			data[1] = pUserData->dAirspeed;
			data[2] = pUserData->dHeight;
			data[3] = pUserData->dPitch * (-57.2957804);
			::PostMessage(hWnd, WM_USER_MSG_HDL, 0, (LPARAM)(data));

			if (g_pFile != NULL)
			{
				char szBuffer[4096] = { 0 };
				time_t t = time(NULL);
				unsigned int nMS = timeGetTime() % 1000;

				if (t > tCurSec)
				{
					tCurSec = t;
					nLastMS = 0;
				}

				if ((nMS - nLastMS) > nTimeOutMS)
				{
					nLastMS = nMS;

					// Stream the user object's data into the CSV file.
					PrintString(szBuffer, sizeof(szBuffer), pUserData->szTitle);

#if USE_NEW_TIME
#else
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAbsoluteTime);
#endif


#if USE_NEW_TIME
					GetTimeStr(szBuffer, sizeof(szBuffer), pUserData->dTime);
#else
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dTime);
#endif
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dSimTime);
					PrintInt(szBuffer, sizeof(szBuffer), pUserData->uDayOfTheYear);
#if USE_NEW_TIME
					GetDateStr(szBuffer, sizeof(szBuffer), pUserData->uYear, pUserData->uMonthOfTheYear, pUserData->uDayOfTheMonth);
#else
					PrintInt(szBuffer, sizeof(szBuffer), pUserData->uYear);
					PrintInt(szBuffer, sizeof(szBuffer), pUserData->uMonthOfTheYear);
					PrintInt(szBuffer, sizeof(szBuffer), pUserData->uDayOfTheMonth);
#endif
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dLatitude);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dLongitude);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dBank * 57.2957804);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dHeading * 57.2957804);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAlpha * 57.5957804);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dBeta * 57.5957804);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAirspeed);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dHeight);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dPitch* (-57.2957804));
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dElevator_def * 57.2957804);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dRudder_def * 57.2957804);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAileron_left * 57.2957804);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dAileron_right * 57.2957804);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindDirection);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindVelocity);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindX);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindY);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dWindZ);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dVerSpeed);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dn1);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dn2);
					GetWindX(szBuffer, sizeof(szBuffer), winddata[0], winddata[1], winddata[2],
						winddata[3], winddata[4], winddata[5], winddata[6]);
					GetWindY(szBuffer, sizeof(szBuffer), winddata[0], winddata[1], winddata[2],
						winddata[3], winddata[4], winddata[5], winddata[7]);
					GetWindZ(szBuffer, sizeof(szBuffer), winddata[0], winddata[1], winddata[2],
						winddata[3], winddata[4], winddata[5], winddata[8]);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dwvelocityx);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dwvelocityy);
					PrintDouble(szBuffer, sizeof(szBuffer), pUserData->dwvelocityz);

					strcat_s(szBuffer, sizeof(szBuffer), "\n");
					fprintf_s(g_pFile, szBuffer);
				}
			}
			break;
		}
		default:
		{
			break;
		}
		}
		break; // SIMCONNECT_RECV_ID_SIMOBJECT_DATA
	}

	case SIMCONNECT_RECV_ID_QUIT:
	{
		g_bQuit = 1;
		break;
	}

	default:
	{
		printf("\nReceived: %d", pData->dwID);
		break;
	}
	}
}
