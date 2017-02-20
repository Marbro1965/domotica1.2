#include "parserIni.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"
#include "simplelink.h"

#define SUCCESS                 0

#define FAILURE                 -1

#include "definitions.h"

#ifndef NOTERM
#include "uart_if.h"
#endif


#define SL_MAX_FILE_SIZE        64L*1024L       /* 64KB file */

#define BUF_SIZE                2048

#define USER_FILE_NAME          "domotica.ini"


unsigned char gaucCmpBuf[BUF_SIZE];

configuration config;

/* Application specific status/error codes */
typedef enum{
    // Choosing this number to avoid overlap w/ host-driver's error codes
    FILE_ALREADY_EXIST = -0x7D0,
    FILE_CLOSE_ERROR = FILE_ALREADY_EXIST - 1,
    FILE_NOT_MATCHED = FILE_CLOSE_ERROR - 1,
    FILE_OPEN_READ_FAILED = FILE_NOT_MATCHED - 1,
    FILE_OPEN_WRITE_FAILED = FILE_OPEN_READ_FAILED -1,
    FILE_READ_FAILED = FILE_OPEN_WRITE_FAILED - 1,
    FILE_WRITE_FAILED = FILE_READ_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
	
}e_AppStatusCodes;




static long ReadFromFile(unsigned long ulToken, long lFileHandle){

    long lRetVal = -1;

    //
    // open a user file for reading
    //
    lRetVal = sl_FsOpen((unsigned char *)USER_FILE_NAME,
                        FS_MODE_OPEN_READ,
                        &ulToken,
                        &lFileHandle);
    if(lRetVal < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        ASSERT_ON_ERROR(FILE_OPEN_READ_FAILED);
    }

    //
    //
    //
    lRetVal = sl_FsRead(lFileHandle,0,gaucCmpBuf, sizeof(gaucCmpBuf));

    if (lRetVal < 0)
    {
        lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
        ASSERT_ON_ERROR(FILE_READ_FAILED);
    }

    //
    // close the user file
    //
    lRetVal = sl_FsClose(lFileHandle, 0, 0, 0);
    if (SL_RET_CODE_OK != lRetVal)
    {
        ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
    }

    return SUCCESS;

}

long WriteToFile(unsigned long *ulToken, long *lFileHandle){

    long lRetVal = -1;

    //
    //  create a user file
    //
    lRetVal = sl_FsOpen((unsigned char *)USER_FILE_NAME,
                FS_MODE_OPEN_CREATE(65536, \
                          _FS_FILE_OPEN_FLAG_COMMIT|_FS_FILE_PUBLIC_WRITE),
                        ulToken,
                        lFileHandle);
    if(lRetVal < 0)
    {
        //
        // File may already be created
        //
        lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
        ASSERT_ON_ERROR(lRetVal);
    }
    else
    {
        //
        // close the user file
        //
        lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
        if (SL_RET_CODE_OK != lRetVal)
        {
            ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
        }
    }

    //
    //  open a user file for writing
    //
    lRetVal = sl_FsOpen((unsigned char *)USER_FILE_NAME,
                        FS_MODE_OPEN_WRITE,
                        ulToken,
                        lFileHandle);
    if(lRetVal < 0)
    {
        lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
        ASSERT_ON_ERROR(FILE_OPEN_WRITE_FAILED);
    }

    //
    //
    //
    lRetVal = sl_FsWrite(*lFileHandle,0,(unsigned char *)gaucCmpBuf, sizeof(gaucCmpBuf));
    if (lRetVal < 0)
        {
        lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
        ASSERT_ON_ERROR(FILE_WRITE_FAILED);
        }
    //
    // close the user file
    //
    lRetVal = sl_FsClose(*lFileHandle, 0, 0, 0);
    if (SL_RET_CODE_OK != lRetVal)
    {
        ASSERT_ON_ERROR(FILE_CLOSE_ERROR);
    }

    return SUCCESS;

}



char* ini_buffer_reader(char* str, int num, void* stream)
{
    buffer_ctx* ctx = (buffer_ctx*)stream;
    int idx = 0;
    char newline = 0;

    if (ctx->bytes_left <= 0)
        return NULL;

    for (idx = 0; idx < num - 1; ++idx)
    {
        if (idx == ctx->bytes_left)
            break;

        if (ctx->ptr[idx] == '\n')
            newline = '\n';
        else if (ctx->ptr[idx] == '\r')
            newline = '\r';

        if (newline)
            break;
    }

    memcpy(str, ctx->ptr, idx);
    str[idx] = 0;

    ctx->ptr += idx + 1;
    ctx->bytes_left -= idx + 1;

    if (newline && ctx->bytes_left > 0 &&
            ((newline == '\r' && ctx->ptr[0] == '\n') ||
             (newline == '\n' && ctx->ptr[0] == '\r'))) {
        ctx->bytes_left--;
        ctx->ptr++;
    }
    return str;
}

int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

    if (MATCH("AP", "SSID"))
    	{
    	memset(pconfig->SSID,0,sizeof(pconfig->SSID));
    	memcpy(pconfig->SSID,value,strlen(value));

    	}
    else if (MATCH("AP", "SECURITY_KEY"))
    	{
    	memset(pconfig->SecurityKey,0,sizeof(pconfig->SecurityKey));
		memcpy(pconfig->SecurityKey,value,strlen(value));

    	}
    else if (MATCH("AP", "SECURITY_TYPE"))
    	{
        pconfig->SecurityType = atoi(value);
    	}
    else if (MATCH("Brokers", "BrokerNumber"))
    	{
    	pconfig->brokerNumber = atoi(value);
    	}
    else if (MATCH("Brokers", "Address1"))
    	{
    	memset(pconfig->address1,0,sizeof(pconfig->address1));
		memcpy(pconfig->address1,value,strlen(value));

    	}
    else if (MATCH("Brokers", "Address2"))
    	{
    	memset(pconfig->address2,0,sizeof(pconfig->address2));
		memcpy(pconfig->address2,value,strlen(value));

    	}
    else if (MATCH("Brokers", "userId1"))
    	{
    	memset(pconfig->userId1,0,sizeof(pconfig->userId1));
		memcpy(pconfig->userId1,value,strlen(value));
    	}
    else if (MATCH("Brokers", "userId2"))
    	{
    	memset(pconfig->userId2,0,sizeof(pconfig->userId2));
		memcpy(pconfig->userId2,value,strlen(value));

    	}
    else if (MATCH("Brokers", "password1"))
    	{
    	memset(pconfig->password1,0,sizeof(pconfig->password1));
		memcpy(pconfig->password1,value,strlen(value));

    	}
    else if (MATCH("Brokers", "password2"))
    	{
    	memset(pconfig->password2,0,sizeof(pconfig->password2));
		memcpy(pconfig->password2,value,strlen(value));

    	}
    else if (MATCH("Topic", "entryPublish"))
    	{
    	memset(pconfig->entryPublish,0,sizeof(pconfig->entryPublish));
		memcpy(pconfig->entryPublish,value,strlen(value));

    	}
    else if (MATCH("Topic", "entrySubscribe"))
    	{
    	memset(pconfig->entrySubscribe,0,sizeof(pconfig->entrySubscribe));
		memcpy(pconfig->entrySubscribe,value,strlen(value));

    	}
    else if (MATCH("Brokers", "port1"))
     	{
     	pconfig->port1 = atoi(value);
     	}
    else if (MATCH("Brokers", "port2"))
      	{
       	pconfig->port2 = atoi(value);
       	}
    else

    	{
        return 0;  /* unknown section/name, error */
    	}
    return 1;
}

static int parseFile()
{

	buffer_ctx ctx;

	ctx.ptr = (const char*)gaucCmpBuf;

	ctx.bytes_left = strlen(ctx.ptr);

	if (ini_parse_stream((ini_reader)ini_buffer_reader, &ctx, handler, &config) < 0) {

		return 1;
		}
	return 0;
}

static int outputFile()
{

//	;File di configurazione per progetto domotica
//
//	[AP] ; Parametri access point
//	SSID = Tina ;
//	SECURITY_KEY = 	F2AY1EL7 ;
//	SECURITY_TYPE = 2 ; SL_SEC_TYPE_WPA_WPA2
//
//	[Brokers] ;
//	BrokerNumber = 1 ;number of broker
//	Address1 = m10.cloudmqtt.com ;
//	Address2 =
//	userId1 = Fortunato
//	userId2 =
//	password1 = Marrelli_2016
//	password2 =
//	port1 = 19964
//	port2 = 1883
//
//	[Topic]
//	entry = /cc3200/ ;entry per ogni scheda

    long lFileHandle;

    unsigned long ulToken;

	char *pInit = 0;

	char *pFile = (char*)gaucCmpBuf;

	pInit = pFile;

	pFile += sprintf(pFile,"%s",";File di configurazione per progetto domotica\r\n");

	pFile += sprintf(pFile,"%s","[AP] ; Parametri access point\r\n");

	pFile += sprintf(pFile,"%s%s%s","SSID = ",config.SSID,"\r\n");

	pFile += sprintf(pFile,"%s%s%s","SECURITY_KEY = ",config.SecurityKey,"\r\n");

	pFile += sprintf(pFile,"%s%d%s","SECURITY_TYPE = ",config.SecurityType,"\r\n");


	//[Brokers]
	pFile += sprintf(pFile,"%s","[Brokers]\r\n");

	pFile += sprintf(pFile,"%s%d%s","BrokerNumber = ",config.brokerNumber,"\r\n");

	pFile += sprintf(pFile,"%s%s%s","Address1 = ",config.address1,"\r\n");

	pFile += sprintf(pFile,"%s%s%s","Address2 = ",config.address2,"\r\n");

	pFile += sprintf(pFile,"%s%s%s","userId1 = ",config.userId1,"\r\n");

	pFile += sprintf(pFile,"%s%s%s","userId2 = ",config.userId2,"\r\n");

	pFile += sprintf(pFile,"%s%s%s","password1 = ",config.password1,"\r\n");

	pFile += sprintf(pFile,"%s%s%s","password2 = ",config.password2,"\r\n");

	pFile += sprintf(pFile,"%s%d%s","port1 = ",config.port1,"\r\n");

	pFile += sprintf(pFile,"%s%d%s","port2 = ",config.port2,"\r\n");
	//
	//	[Topic]
	pFile += sprintf(pFile,"%s","[Topic]\r\n");

	pFile += sprintf(pFile,"%s%s%s","entryPublish = ",config.entryPublish,"\r\n");

	pFile += sprintf(pFile,"%s%s%s","entrySubscribe = ",config.entrySubscribe,"\r\n");

    if(WriteToFile(&ulToken, &lFileHandle) < 0)
    {
    	return 0;
    }


	return pFile-pInit;

}

