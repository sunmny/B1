#ifndef __BD_TF_H__
#define __BD_TF_H__

#define ST_OK							0x00000000	//成功
#define ST_FAIL						    0x08000001	//失败
#define ST_UNKNOWNERR					0x08000002	//异常错误
#define ST_NOTSUPPORTYETERR			    0x08000003	//不支持的服务
#define ST_CHECKSUMERR			        0x08000004	//杂凑校验码错误
#define ST_INVALIDHANDLEERR			    0x08000005	//无效的句柄
#define ST_INVALIDPARAMERR				0x08000006	//无效的参数
#define ST_PUCIDMATCHERR			    0x08000007	//终端标识不匹配
#define ST_UCVERSIONMATCHERR			0x08000008	//协议版本号不匹配
#define ST_NAMELENERR					0x08000009	//名称长度错误
#define ST_NOTINITIALIZEERR 			0x0800000C	//未初始化
#define ST_OBJERR						0x0800000D	//对象错误
#define ST_INDATALENERR				    0x08000010	//输入数据长度错误
#define ST_INDATAERR					0x08000011	//输入数据错误
#define ST_GENRANDERR					0x08000012	//生成随机数错误
#define ST_KEYNOTFOUNTERR				0x0800001B	//未发现密钥
#define ST_NOTEXPORTERR				    0x0800001D	//对象未导出
#define ST_BUFFER_TOO_SMALL			    0x08000020	//缓冲区不足
#define ST_KEYINFOTYPEERR				0x08000021	//密钥类型错误
#define ST_DEVICE_REMOVED				0x08000023	//设备已移除

#ifdef  __cplusplus
extern "C" {
#endif
	
//获取终端索引号
int TF_GetDeviceID(
        char *pucID,                //??,????????
        unsigned int *puiIDLen      //??,???????
    );

//使用设备传输保护密钥加密数据
int TF_EncData(
        unsigned char *pucData,     //输入，明文
        unsigned int uiDataLen,     //输入，明文长度
        unsigned char *pucCData,    //输出，密文
        unsigned int *puicDataLen   //输出，密文长度
    );

//使用设备传输保护密钥解密数据
int TF_DecData(
        unsigned char *pucCData,    //输入，密文
        unsigned int uicDataLen,    //输入，密文长度
        unsigned char *pucData,     //输出，明文
        unsigned int *puiDataLen    //输出，明文长度
    );

#ifdef  __cplusplus
}
#endif

#endif   