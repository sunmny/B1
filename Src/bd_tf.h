#ifndef __BD_TF_H__
#define __BD_TF_H__

#define ST_OK							0x00000000	//�ɹ�
#define ST_FAIL						    0x08000001	//ʧ��
#define ST_UNKNOWNERR					0x08000002	//�쳣����
#define ST_NOTSUPPORTYETERR			    0x08000003	//��֧�ֵķ���
#define ST_CHECKSUMERR			        0x08000004	//�Ӵ�У�������
#define ST_INVALIDHANDLEERR			    0x08000005	//��Ч�ľ��
#define ST_INVALIDPARAMERR				0x08000006	//��Ч�Ĳ���
#define ST_PUCIDMATCHERR			    0x08000007	//�ն˱�ʶ��ƥ��
#define ST_UCVERSIONMATCHERR			0x08000008	//Э��汾�Ų�ƥ��
#define ST_NAMELENERR					0x08000009	//���Ƴ��ȴ���
#define ST_NOTINITIALIZEERR 			0x0800000C	//δ��ʼ��
#define ST_OBJERR						0x0800000D	//�������
#define ST_INDATALENERR				    0x08000010	//�������ݳ��ȴ���
#define ST_INDATAERR					0x08000011	//�������ݴ���
#define ST_GENRANDERR					0x08000012	//�������������
#define ST_KEYNOTFOUNTERR				0x0800001B	//δ������Կ
#define ST_NOTEXPORTERR				    0x0800001D	//����δ����
#define ST_BUFFER_TOO_SMALL			    0x08000020	//����������
#define ST_KEYINFOTYPEERR				0x08000021	//��Կ���ʹ���
#define ST_DEVICE_REMOVED				0x08000023	//�豸���Ƴ�

#ifdef  __cplusplus
extern "C" {
#endif
	
//��ȡ�ն�������
int TF_GetDeviceID(
        char *pucID,                //??,????????
        unsigned int *puiIDLen      //??,???????
    );

//ʹ���豸���䱣����Կ��������
int TF_EncData(
        unsigned char *pucData,     //���룬����
        unsigned int uiDataLen,     //���룬���ĳ���
        unsigned char *pucCData,    //���������
        unsigned int *puicDataLen   //��������ĳ���
    );

//ʹ���豸���䱣����Կ��������
int TF_DecData(
        unsigned char *pucCData,    //���룬����
        unsigned int uicDataLen,    //���룬���ĳ���
        unsigned char *pucData,     //���������
        unsigned int *puiDataLen    //��������ĳ���
    );

#ifdef  __cplusplus
}
#endif

#endif   