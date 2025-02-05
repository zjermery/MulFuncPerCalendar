#ifndef ERROR_CODE_H
#define ERROR_CODE_H 

#include <stdint.h>

typedef uint32_t error_code_t;

#define    ERROR_CODE_SUCCESS             0x00000000                 // �����ɹ�
#define    ERROR_CODE_ERROR               0xFFFFFFFF                 // ͨ�ô���
#define    ERROR_CODE_TIMEOUT             0x00000001                 // ��ʱ����
#define    ERROR_CODE_INVALID_PARAMETER   0x00000002                 // ��Ч����
#define    ERROR_CODE_NOT_INITIALIZED     0x00000003                 // δ��ʼ��
#define    ERROR_CODE_DEVICE_NOT_FOUND    0x00000004                 // �豸δ�ҵ�
#define    ERROR_CODE_COMMUNICATION_ERROR 0x00000005                 // ͨ�Ŵ���
#define    ERROR_CODE_BUFFER_OVERFLOW     0x00000006                 // ���������
#define    ERROR_CODE_OUT_OF_MEMORY       0x00000007                 // �ڴ治��
#define    ERROR_CODE_INVALID_STATE       0x00000008                 // ��Ч״̬
#define    ERROR_CODE_PERMISSION_DENIED   0x00000009                 // Ȩ�ޱ��ܾ�
#define    ERROR_CODE_UNKNOWN_COMMAND     0x0000000A                 // δ֪����
#define    ERROR_CODE_HARDWARE_ERROR      0x0000000B                 // Ӳ������
#define    ERROR_CODE_CHECKSUM_ERROR      0x0000000C                 // У��ʹ���
#define    ERROR_CODE_INVALID_DATA        0x0000000D                 // ��Ч����
#define    ERROR_CODE_BUSY                0x0000000E                 // �豸��æ
#define    ERROR_CODE_NOT_SUPPORTED       0x0000000F                 // ���ܲ�֧��
#define    ERROR_CODE_INTERRUPTED         0x00000010                 // �������ж�
#define    ERROR_CODE_INTERNAL_ERROR      0x00000011                 // �ڲ�����

#define    DEV_ERROR_CODE_BASE            0x80001000                 // �豸�����ַ
#define    ERROR_CODE_DEV_NOT_FOUND       0x80001001                 // �豸δ�ҵ�
#define    ERROR_CODE_DEV_NOT_SUPPORT     0x80001002                 // �豸��֧��


#endif // ERROR_H

