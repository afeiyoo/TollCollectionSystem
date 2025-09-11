#include "windows.h"
#pragma once
struct EtcExitInfo {
    /*
    �������ƺ���+��ɫ.
    ���ƺ���+�����+������ɫ
    ���������_��,������ɫ2λ����:
     0-��ɫ��1-��ɫ��2-��ɫ��3-��ɫ��4- ������ɫ,5- ����˫ƴɫ,6- ���׽���ɫ,7- ��ʱ����,11-��ɫ,12-��ɫ
    ������A12345_1
    */
    char* vehicleId;

    /* ����շ�վ��� */
    char* enStationId;

    /* �����շ�վ��� */
    char* exStationId;

    /* �������.�������أ���λ��KG */
    int enWeight;

    /* ������������û������-1.�������أ���λ��KG */
    int exWeight;

    /* ͨ�н���  1λ���� 1-OBU,2-CPC��,3-ֽȯ,9-��ͨ�н���*/
    int mediaType;

    /* ���ڽ��ױ��*/
    char* transactionId;

    /* �շ�ϵͳ�в����ĳ���ͨ��ID.ͨ��ID=ͨ�н���ID+���ʱ�� */
    char* passId;

    /* ���ڽ���ʱ�� YYYY-MM-DDTHH:mm:ss*/
    char* exTime;

    /* ����֧����ʽ ETC����ʱ����������׿�Ϊ�գ�������Ҫ�����ָ���1λ����,1-����ETCͨ��,2-����ETCˢ��ͨ��*/
    int transPayType;

    /* �ܽ��׽�� ����ͨ��ʡ�ݲ�ֵĽ��*/
    long fee;

    /* Ӧ�ս�� ��������ʱ���ETC���׿�Ϊ��,��λ���֡�Ӧ�ս��=���׽��+�Żݽ��+ʡ�����Żݽ��*/
    long payFee;

    /* ����״̬��ʶ. 1λ����. 0x02-��ͨ����0x03-�����ո����0xFF-Ĭ��ֵ */
    char* vehicleSign;

	char* provinceCount;
};

/*
���ɲ����APP��Ҫɨ��ĳ�����Ϣ��ά�룬����HBITMAP-Windowsλͼ����ľ��
@param text gbk��ʽ������ַ�����������Զ�����ת��Ϊutf-8����
@width ��ά��Ŀ��
*/
// dll��generateQrCodeAndReturnHBITMAP������ָ��
typedef HBITMAP (__stdcall* function_generateQrCodeAndReturnHBITMAP)(EtcExitInfo etcExitInfo, int width);

