/*
LazyPCSCfelicaLite v0.41

zlib License
Copyright (c) 2018 GPS_NMEA_JP

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

/*
v0.41
autoConnectToFelica�ŁCFelica�ł͂Ȃ��J�[�h�����o�����ۂɗ�O��f���s����C��

v0.4

PCSCCryptographicException��ǉ��D
Felica Lite-S�̓����A�h���X��萔��
readBinary�̉����`�F�b�N�����i��(�T�C�Y�`�F�b�N)
readBinaryWithMAC��ǉ��DMAC�t���ǂݎ�肪�ł���悤��
updateBinaryWithMAC_A��ǉ��DMAC�t���������݂��ł���悤��
�������ݎ��Ƀ����������x�����G���[�Ƃ��Ĉ���Ȃ��悤�ɕύX
�������ݎ��ɏ������ݓ��e���f�o�b�O�\������悤�ɕύX

MAC���،n���߂�ǉ�
desEncryptFixedLength
desDecryptFixedLength
swapByteOrder
showMAC
arryXor
zeroIV
Key2des3
DualKey2des3
makeSessionKey
makeMAC
makeReadMAC_A
makeWriteMAC_A
compareMAC
compareMAC_A
writeRandomChallenge
writeCardKey
cardIdCheckMAC
cardIdCheckMAC_A
makeRandomChallenge
enableSTATE
setSTATE_EXT_AUTH
updateBinaryWithMAC_A_Auto
CryptRand
��ǉ�

v0.31
�V�X�e���R�[�h�擾�̊֌W�̃}�W�b�N�i���o�[��萔��
�擾�ɂ�50ms����Ώ\���Ȃ��Ƃ��킩�����̂�50ms�Ń^�C���A�E�g�ɁD
isCardSet()��readSystemcode()��������(�J�[�h�̎��O����isCardSet�Ō��o�ł��Ȃ�)���Ƃ������D���Ώ��ɂ̓|�[�����O�Ԋu���̑҂����Ԃ�����K�v������...
readTypecode�̓����\�L���C���D
��O�N���X����C++�K���ᔽ���C���D
�f�o�b�O�\���̊֐����~�X�o�ł̂��ߎd�l��ύX�D
�J�[�h��ʂȂǂ̃L���b�V���̏����l���C��
connect���ɃV�X�e���R�[�h�ƃJ�[�h��ʂ��L���b�V������ۂɃ~�X����Felica�ȊO���󂯕t���Ȃ��Ȃ��Ă����̂��C��
�C���X�^���X����������悤�ɁD setInstanceName��ǉ��D
��O�N���XPCSCCardRemovedException��ǉ��D�J�[�h���O���n�͈Ȍケ�����
���g���C���폜
autoConnectToFelica��ǉ��D�ڑ��֌W�̏����������ōs���C�����ɑΏ��D

v0.3
getSystemcode��readSystemcode��
getCardType��readCardType��
connect���ɃV�X�e���R�[�h�ƃJ�[�h��ʂ��L���b�V������悤��
connect���̃V�X�e���R�[�h�擾��S��悤��(�����ȋ������Ǝ��s���邽��)

v0.2
polling��ǉ�
getCurrentState��ǉ�
���d�ڑ��h�~������ǉ�
isCard�̃o�O(�������g�p���Ă��Ă����҂��g�p���Ɍ�����)���C��

v0.1
����

���f�t�H���g�̐ݒ肾�ƁC�^�b�`���甽���܂ŏ��X���Ԃ�������܂��D
�@�R���g���[���p�l���́uNFC�|�[�g�^�p�\���v�ɂāC
 �@�E�u�|�[�����O�Ԋu�v���u�����ɂ���v
  �@�E�u���o�ΏۃJ�[�h�v���uNFC-F�v�݂̂ɂ���
   �@�ƁC���D�@���݂̔����ɂȂ�܂��D
	�@(�������C���o�ΏۃJ�[�h���i���e-tax��}�C�i���o�[�|�[�^���Ŏg���Ȃ��Ȃ�܂��D)

	 �uFeliCa�v����сuPaSoRi�v����сu�p�\���v�̓\�j�[������Ђ̓o�^���W�ł��B
	 �uMIFARE�v��NXP�Z�~�R���_�N�^�[�Y�̓o�^���W�ł��B
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>

#include <winscard.h>
#pragma comment (lib, "winscard.lib")
#include <Bcrypt.h>
#pragma comment (lib, "Bcrypt.lib")

namespace LazyPCSCFelicaLite
{
	using std::vector;
	using std::string;

	// PC/SC�̃G���[�����D�ʏ�͔������Ȃ��D�R�}���h��肩��Ԃ��ُ�
	//�v���O�����~�X���@��̏���^������
	class PCSCErrorException : public std::runtime_error
	{
	public:
		PCSCErrorException(const char* Message, uint8_t SW1, uint8_t SW2)
			: Errinfo(), runtime_error(Message)
		{
			Errinfo.SW1 = SW1;
			Errinfo.SW2 = SW2;
		}

		uint8_t sw1()
		{
			return Errinfo.SW1;
		}
		uint8_t sw2()
		{
			return Errinfo.SW2;
		}
	private:
		struct PCSCError_struct
		{
			uint8_t SW1;
			uint8_t SW2;
		};
		PCSCError_struct Errinfo;
	};

	// PC/SC�̊֐��G���[�D�܂��߂����ɔ������Ȃ��D(���ڐڑ����ɃJ�[�h�֌W����������ꍇ������)
	//�v���O�����~�X���@��̏���^������
	class PCSCCommandException : public std::runtime_error
	{
	public:
		PCSCCommandException(const char* Message, LONG res)
			: Errinfo(res), runtime_error(Message)
		{}

		LONG returncode()
		{
			return Errinfo;
		}
	private:
		LONG Errinfo;
	};

	//�{������ׂ��ł͂Ȃ���ԂŎ��s���悤�Ƃ����D
	//�v���O�����~�X
	class PCSCIllegalStateException : public  std::runtime_error
	{
	public:
		PCSCIllegalStateException(const char* Message)
			: runtime_error(Message)
		{}
	};

	//�J�[�h���Ȃ��C�J�[�h���r���Ŏ��O���ꂽ
	//�J�[�h���Z�b�g����Ă��Ȃ���Ԃ�connectCard()���Ăяo�����ꍇ�͂��Ȃ炸�o��D
	class PCSCCardRemovedException : public std::runtime_error
	{
	public:
		PCSCCardRemovedException(const char* Message)
			: runtime_error(Message)
		{}
	};

	//�Í��������G���[
	class PCSCCryptographicException : public std::runtime_error
	{
	public:
		PCSCCryptographicException(const char* Message)
			: runtime_error(Message)
		{}
	};

	//�J�[�h���[�_�[���Ȃ��C�J�[�h���ُ�ȉ�����Ԃ����CPC/SC SERVICE�ɐڑ��ł��Ȃ��ȂǁC
	//�p���s�\�ȁC�V�X�e���I���邢�͕����I�ȃ����^�C�����
	class PCSCFatalException : public std::runtime_error
	{
	public:
		enum
		{
			FAILD_TO_ESTABLISH,
			NO_READERS_AVAILABLE,
			FAILED_TO_DETECT_READER,
			FAILED_TO_GET_CARD_STATUS,
			READER_DISCONNECTED,
			UNKNOWN_STATUS,
			UNKNOWN_ERROR,
			FAILED_TO_DIRECT_CONNECTION,
			CARD_HAS_LOCKED_BY_ANOTHER_PROGRAM,
			READER_NOT_FOUND,
			INVALID_READER_NAME,
		};

		PCSCFatalException(const char* Message, int errorcode, LONG ret)
			: Errinfo(), runtime_error(Message)
		{
			Errinfo.errorcode = errorcode;
			Errinfo.ret = ret;
		}

		int errorcode()
		{
			return Errinfo.errorcode;
		}
		LONG returncode()
		{
			return Errinfo.ret;
		}
	private:
		struct PCSCFatalError_struct
		{
			int errorcode;
			LONG ret;
		};

		PCSCFatalError_struct Errinfo;
	};

	//Felica�J�[�h���G���[�D
	//�������݋֎~�̈�ɏ����������Ƃ�����C�F�؂��Ȃ��ŗv�F�ؗ̈�ɏ����������Ƃ����ꍇ�ȂǁD
	//��{�I�Ƀv���O�����~�X�DSony��Felica�d�l�����悭�ǂނ��ƁD
	//�G���[���b�Z�[�W�ɏڍ׏�񂪏�����Ă���
	class FelicaErrorException : public std::runtime_error
	{
	public:
		FelicaErrorException(const char* Message, uint8_t High, uint8_t Low)
			: Errinfo(), runtime_error(Message)
		{
			Errinfo.High = High;
			Errinfo.Low = Low;
		}

		uint8_t high()
		{
			return Errinfo.High;
		}
		uint8_t low()
		{
			return Errinfo.Low;
		}
	private:
		struct PCSCError_struct
		{
			uint8_t High;
			uint8_t Low;
		};
		PCSCError_struct Errinfo;
	};

	//Felica�J�[�h��Ԉُ�
	//1�����s����Ă���J�[�h�ɑ΂��Ĕ��s�֌W�̏��������悤�Ƃ����Ȃ�
	class FelicaFatalException : public std::runtime_error
	{
	public:
		enum
		{
			ALREADY_FIRST_ISSUED,
			CARD_IS_NOT_FELICA_LITE,
			INVALID_RESPONSE,
			WCNT_CLIP
		};

		FelicaFatalException(const char* Message, int errorcode)
			: Errinfo(errorcode), runtime_error(Message)
		{}

		int errorcode()
		{
			return Errinfo;
		}
	private:
		int Errinfo;
	};

	//---------------------------------------


	class PCSCFelicaLite
	{
	private:
		SCARDCONTEXT hContext = NULL;
		LPSTR       lpszReaderName = NULL;
		DWORD        readerNameListSize = 0;
		SCARDHANDLE  hCard = NULL;
		SCARD_READERSTATEA readerState;

		string InstanceName = "PCSCFelicaLite";
		uint16_t cardSystemcode = SYSTEMCODE_ANY;
		uint8_t cardTypecode = CARD_TYPE_UNKNOWN;

		vector<string> ReaderNameList;
		string activeReaderName = "";

		bool debugFlag = false;
		void debugInfo2(const char* funcname, const char* fmt, ...)
		{
			if (debugFlag == false)
			{
				return;
			}
			printf("[%s.%s]", InstanceName.c_str(), funcname);
			va_list va;
			va_start(va, fmt);
			vprintf(fmt, va);
			va_end(va);
		}

	public:
		enum
		{
			ESC_CMD_GET_INFO = 0xC0, //�o�[�W�����ȂǊe����̎擾
			ESC_CMD_SET_OPTION = 0xC1, //���̐ݒ�
			ESC_CMD_TARGET_COMM = 0xC4, //�^�[�Q�b�g�ʐM
			ESC_CMD_SNEP = 0xC6, //SNEP�ʐM
			ESC_CMD_APDU_WRAP = 0xFF, //PC / SC 2.02��APDU�p���b�p

									  //ESC_CMD_GET_INFO
									  DRIVER_VERSION = 0x01, //�h���C�o�o�[�W����(AA.BB.CC.DD)
									  FW_VERSION = 0x02,
									  VENDOR_ID = 0x04,
									  VENDOR_NAME = 0x05,
									  PRODUCT_ID = 0x06,
									  PRODUCT_NAME = 0x07,
									  PRODUCT_SERIAL_NUMBER = 0x08,
									  CAPTURED_CARD_ID = 0x10, //0=UNCAPTURED, FF=Unknown
									  NFC_DEP_ATR_REG_GENERAL_BYYES = 0x12,

									  //----------APDU------------
									  APDU_CLA_GENERIC = 0xFF,

									  APDU_INS_GET_DATA = 0xCA,
									  APDU_INS_READ_BINARY = 0xB0,
									  APDU_INS_UPDATE_BINARY = 0xD6,
									  APDU_INS_DATA_EXCHANGE = 0xFE,
									  APDU_INS_SELECT_FILE = 0xA4,

									  APDU_P2_NONE = 0x00,

									  //----APDU_INS_GET_DATA----
									  APDU_P1_GET_UID = 0x00,
									  APDU_P1_GET_PMm = 0x01,
									  APDU_P1_GET_CARD_ID = 0xF0,
									  APDU_P1_GET_CARD_NAME = 0xF1,
									  APDU_P1_GET_CARD_SPEED = 0xF2,
									  APDU_P1_GET_CARD_TYPE = 0xF3,
									  APDU_P1_GET_CARD_TYPE_NAME = 0xF4,
									  APDU_P1_NFC_DEP_TARGET_STATE = 0xF9,

									  APDU_LE_MAX_LENGTH = 0x00,

									  UID_SIZE_ISO14443B = 4,
									  UID_SIZE_PICOPASS = 8,
									  UID_SIZE_NFCTYPE1 = 7,
									  UID_SIZE_FELICA = 8,

									  CARD_SPEED_106KBPS = 0x01,
									  CARD_SPEED_212KBPS = 0x02,
									  CARD_SPEED_424KBPS = 0x03,
									  CARD_SPEED_848KBPS = 0x04,

									  CARD_TYPE_UNKNOWN = 0x00,
									  CARD_TYPE_ISO14443A = 0x01,
									  CARD_TYPE_ISO14443B = 0x02,
									  CARD_TYPE_PICOPASSB = 0x03,
									  CARD_TYPE_FELICA = 0x04,
									  CARD_TYPE_NFC_TYPE_1 = 0x05,
									  CARD_TYPE_MIFARE_EC = 0x06,
									  CARD_TYPE_ISO14443A_4A = 0x07,
									  CARD_TYPE_ISO14443B_4B = 0x08,
									  CARD_TYPE_TYPE_A_NFC_DEP = 0x09,
									  CARD_TYPE_FELICA_NFC_DEP = 0x0A,

									  //APDU_INS_READ_BINARY
									  USE_BLOCKLIST = 0x80,
									  NO_RFU = 0,

									  //----------APDU_INS_DATA_EXCHANGE-----
									  APDU_P1_THRU = 0x00,
									  APDU_P1_DIRECT = 0x01,
									  APDU_P1_NFC_DEP = 0x02,
									  APDU_P1_DESELECT = 0xFD,
									  APDU_P1_RELEASE = 0xFE,

									  APDU_P2_TIMEOUT_AUTO = 0x00,
									  APDU_P2_TIMEOUT_50MS = 0x05,
									  APDU_P2_TIMEOUT_INFINITY = 0xFF,

									  EXCHANGE_POLLING_PACKET_SIZE = 5,
									  EXCHANGE_POLLING = 0x00,
									  POLLING_REQUEST_SYSTEM_CODE = 0x01,
									  POLLING_TIMESLOT_16 = 0x0F,

									  //---------NDEF-----------
									  NDEF_HTTP_WWW = 0x01,
									  NDEF_HTTPS_WWW = 0x02,
									  NDEF_HTTP = 0x03,
									  NDEF_HTTPS = 0x04,

									  //--------Felica-----
									  SYSTEMCODE_ANY = 0xFFFF,
									  SYSTEMCODE_FELICALITE = 0x88B4,
									  SYSTEMCODE_NFC_TYPE3 = 0x12FC,

									  ADDRESS_SPAD0 = 0x00,
									  ADDRESS_SPAD1 = 0x01,
									  ADDRESS_SPAD2 = 0x02,
									  ADDRESS_SPAD3 = 0x03,
									  ADDRESS_SPAD4 = 0x04,
									  ADDRESS_SPAD5 = 0x05,
									  ADDRESS_SPAD6 = 0x06,
									  ADDRESS_SPAD7 = 0x07,
									  ADDRESS_SPAD8 = 0x08,
									  ADDRESS_SPAD9 = 0x09,
									  ADDRESS_SPAD10 = 0x0A,
									  ADDRESS_SPAD11 = 0x0B,
									  ADDRESS_SPAD12 = 0x0C,
									  ADDRESS_SPAD13 = 0x0D,
									  ADDRESS_RC = 0x80,
									  ADDRESS_MAC = 0x81,
									  ADDRESS_ID = 0x82,
									  ADDRESS_D_ID = 0x83,
									  ADDRESS_SER_C = 0x84,
									  ADDRESS_SYS_C = 0x85,
									  ADDRESS_CKV = 0x86,
									  ADDRESS_CK = 0x87,
									  ADDRESS_MC = 0x88,
									  ADDRESS_WCNT = 0x90,
									  ADDRESS_MAC_A = 0x91,
									  ADDRESS_STATE = 0x92,
									  ADDRESS_CRC = 0xA0,
		};

		PCSCFelicaLite()
		{
			debugFlag = false;
			debugInfo2(__func__, " PCSCFelicaLite()\n");
		}

		PCSCFelicaLite(bool debug)
		{
			debugFlag = debug;
			debugInfo2(__func__, " PCSCFelicaLite(bool debug)\n");
		}

		PCSCFelicaLite(bool debug, string name)
		{
			debugFlag = debug;
			InstanceName = name;
			debugInfo2(__func__, " PCSCFelicaLite(bool debug,string name)\n");
		}

		~PCSCFelicaLite()
		{
			//�����N���[�Y
			debugInfo2(__func__, " �f�R���X�g���N�^\n");
			disconnectCard();
			closeService();
		}

		SCARDHANDLE getCardHandle()
		{
			return hCard;
		}
		SCARDCONTEXT getCardContext()
		{
			return hContext;
		}

		vector<string> getReaderNameList()
		{
			return ReaderNameList;
		}
		string getActiveReaderName()
		{
			return activeReaderName;
		}
		void setActiveReaderName(string s)
		{
			activeReaderName = s;
			debugInfo2(__func__, " %s���g�p���܂�\n", activeReaderName.c_str());
		}

		void setDebug(bool mode)
		{
			debugFlag = mode;
		}

		void setInstanceName(string name) {
			InstanceName = name;
		}

		void checkReadyContext()
		{
			if (hContext == NULL)
			{
				//				throw PCSCFatalException("�X�}�[�g�J�[�h���\�[�X�}�l�[�W���ɂ܂��ڑ����Ă��Ȃ�", PCSCFatalException::NOT_CONNECTED_TO_SERVICE, 0);
				throw PCSCIllegalStateException("�X�}�[�g�J�[�h���\�[�X�}�l�[�W���ɂ܂��ڑ����Ă��Ȃ�");
			}
		}

		void checkReadyCard()
		{
			if (hContext == NULL)
			{
				//				throw PCSCFatalException("�X�}�[�g�J�[�h���\�[�X�}�l�[�W���ɂ܂��ڑ����Ă��Ȃ�", PCSCFatalException::NOT_CONNECTED_TO_SERVICE, 0);
				throw PCSCIllegalStateException("�X�}�[�g�J�[�h���\�[�X�}�l�[�W���ɂ܂��ڑ����Ă��Ȃ�");
			}
			if (hCard == NULL)
			{
				//				throw PCSCFatalException("�J�[�h�ɂ܂��ڑ����Ă��Ȃ�", PCSCFatalException::NOT_CONNECTED_TO_CARD, 0);
				throw PCSCIllegalStateException("�J�[�h�ɂ܂��ڑ����Ă��Ȃ�");
			}
		}


		//------PC/SC����------

		//�ڑ��E�ؒf�E�J�[�h�̌��o�܂ł������ōs���D
		//�J�[�h�֐ڑ��ł����true�C�J�[�h�����݂��Ȃ����false�C�J�[�h���[�_�[���Ȃǂɖ�肪����Η�O
		bool autoConnectToFelica(uint32_t timeout = 0)
		{
			debugInfo2(__func__, "�J�[�h�ւ̎����ڑ��J�n\n");
			//�R���e�L�X�g���Ȃ���Ύ����ŃT�[�r�X���J��
			if (hContext == NULL)
			{
				openService();
			}
			//�T���O�ɃJ�[�h���[�_�[�𗘗p�\�ɂ��Ă���
			disconnectCard();

			if (timeout != 0)
			{
				waitForSetCard(timeout);
			}

			//�J�[�h��T��
			if (!isCardSet())
			{
				debugInfo2(__func__, "�J�[�h��������Ȃ�\n");
				return false;
			}

			disconnectCard();
			//�J�[�h���[�_�[�֐ڑ�

			try {
				connectDirect();
			}
			catch (PCSCFatalException e)
			{
				if (PCSCFatalException::CARD_HAS_LOCKED_BY_ANOTHER_PROGRAM)
				{
					Sleep(100); //����҂�����
					connectDirect();
				}
				else {
					throw e;
				}
			}

			//�|�[�����O�ɐ�������΃J�[�h�����݁C�Ȃ���Ύ��s
			try {
				polling();
			}
			catch (PCSCCardRemovedException e) {
				disconnectCard();
				debugInfo2(__func__, "�J�[�h��������Ȃ�\n");
				return false;
			}
			catch (FelicaFatalException e)
			{
				debugInfo2(__func__, "Felica�ł͂Ȃ��J�[�h�����o����\n");
				return false;
			}
			disconnectCard();

			//�J�[�h���m���ɑ��݂���̂ŃJ�[�h�ɐڑ��D
			try
			{
				connectCard();
			}
			catch (PCSCCardRemovedException e)
			{
				//�ڑ��������ɊO���ꂽ
				disconnectCard();
				debugInfo2(__func__, "�J�[�h��������Ȃ�\n");
				return false;
			}
			debugInfo2(__func__, "�J�[�h�ւ̐ڑ��ɐ���\n");
			return true;
		}


		//�X�}�[�g�J�[�h���\�[�X�}�l�[�W���ւ̐ڑ��ƁC�J�[�h���[�_�[�̎擾
		void openService()
		{
			LONG res;

			closeService();//���d�Ăяo���΍�

						   //---�T�[�r�X�ɐڑ�---
			res = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hContext);
			if (res != SCARD_S_SUCCESS)
			{
				hContext = NULL;
				debugInfo2(__func__, " �X�}�[�g�J�[�h���\�[�X�}�l�[�W���ւ̐ڑ����s:%X\n", res);
				throw PCSCFatalException("�X�}�[�g�J�[�h���\�[�X�}�l�[�W���ւ̐ڑ��Ɏ��s", PCSCFatalException::FAILD_TO_ESTABLISH, res);
			}
			debugInfo2(__func__, " �X�}�[�g�J�[�h���\�[�X�}�l�[�W���ւ̐ڑ��ɐ���\n");

			//---�J�[�h���[�_�[���X�g�̎擾---
			DWORD dwAutoAllocate = SCARD_AUTOALLOCATE;
			res = SCardListReadersA(hContext, NULL, (LPSTR)& lpszReaderName, &dwAutoAllocate);
			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_E_NO_READERS_AVAILABLE)
				{
					debugInfo2(__func__, " �J�[�h���[�_���ڑ�����Ă��܂���B\n");
					throw PCSCFatalException("�J�[�h���[�_���ڑ�����Ă��܂���B", PCSCFatalException::NO_READERS_AVAILABLE, res);
				}
				else
				{
					debugInfo2(__func__, " �G���[�ɂ��J�[�h���[�_�̌��o�Ɏ��s:%X\n", res);
					throw PCSCFatalException("�G���[�ɂ��J�[�h���[�_�̌��o�Ɏ��s", PCSCFatalException::FAILED_TO_DETECT_READER, res);
				}
			}
			readerNameListSize = dwAutoAllocate;

			//---�J�[�h���[�_�[���𒊏o��vector�ɕϊ�---
			vector<char> buf;
			ReaderNameList.clear();
			for (unsigned int i = 0; i < (readerNameListSize - 1); i++)
			{
				buf.push_back(lpszReaderName[i]);
				if (lpszReaderName[i] == '\0')
				{
					//������ɕϊ����ē���
					ReaderNameList.push_back(string(buf.begin(), buf.end()));
					buf.clear();
				}
			}

			//�J�[�h���[�_�[���͂��͂�s�v
			SCardFreeMemory(hContext, lpszReaderName);
			lpszReaderName = NULL;

			//�\��
			if (debugFlag)
			{
				debugInfo2(__func__, " �J�[�h���[�_�[�����X�g\n");
				int i = 0;
				for (string name : ReaderNameList)
				{
					debugInfo2(__func__, "[%d] %s\n", i, name.c_str());
					i++;
				}
			}


			//�J�[�h���[�_�[���̌���
			activeReaderName = "";
			for (string name : ReaderNameList)
			{
				if (name.find("FeliCa") != string::npos)
				{
					activeReaderName = name;
				}
			}

			if (activeReaderName == "")
			{
				debugInfo2(__func__, " ���p�ł���J�[�h���[�_����������܂���ł���\n");
				throw PCSCFatalException("���p�ł���J�[�h���[�_����������܂���ł���", PCSCFatalException::READER_NOT_FOUND, res);
			}
			debugInfo2(__func__, " %s���g�p���܂�\n", activeReaderName.c_str());
		}

		//�X�}�[�g�J�[�h���\�[�X�}�l�[�W������̐ؒf
		void closeService()
		{
			//�J�[�h�ɐڑ����Ă�����ؒf����
			disconnectCard();

			//���
			if (hContext != NULL)
			{
				SCardReleaseContext(hContext);
				debugInfo2(__func__, " �X�}�[�g�J�[�h���\�[�X�}�l�[�W�������\n");
			}

			hContext = NULL;

		}

		//���݂̏�Ԃ��擾����(���Ɏw�肵�Ȃ���Ό��݂̏�Ԃ��擾���đ��߂�)
		bool isCardSet(uint32_t state = SCARD_STATE_UNAWARE, uint32_t timeout = 0)
		{
			checkReadyContext(); //�R���e�L�X�g�����p�\�`�F�b�N(�_���Ȃ��O)

			readerState.szReader = (LPCSTR)activeReaderName.c_str();//lpszReaderName;
			readerState.dwCurrentState = (DWORD)state;


			if (timeout == INFINITE)
				debugInfo2(__func__, " ��Ԃ̎擾���J�n(timeout=�����ɑҋ@)\n");
			else if (timeout == 0)
				;
			else
				debugInfo2(__func__, " ��Ԃ̎擾���J�n(timeout=%dms)\n", timeout);

			LONG res = SCardGetStatusChangeA(hContext, (DWORD)timeout, &readerState, 1);
			if (res == SCARD_E_TIMEOUT)
			{
				debugInfo2(__func__, " �ǂݎ��^�C���A�E�g\n");
				return false;
			}

			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_E_UNKNOWN_READER)
				{
					debugInfo2(__func__, " �J�[�h���[�_�[�����s��:%X\n", res);
					throw PCSCFatalException("�J�[�h���[�_�[�����s��", PCSCFatalException::INVALID_READER_NAME, res);
				}
				debugInfo2(__func__, " �J�[�h��Ԃ̎擾�Ɏ��s:%X\n", res);
				throw PCSCFatalException("�J�[�h��Ԃ̎擾�Ɏ��s", PCSCFatalException::FAILED_TO_GET_CARD_STATUS, res);
			}

			//��Ԃ��X�V�D
			readerState.dwCurrentState = readerState.dwEventState;

			if (readerState.dwEventState & SCARD_STATE_UNAVAILABLE)
			{
				debugInfo2(__func__, " �J�[�h���[�_�[���ڑ�����Ă��Ȃ������p�s�\\n");
				throw PCSCFatalException("�J�[�h���[�_�[���ڑ�����Ă��Ȃ������p�s�\", PCSCFatalException::READER_DISCONNECTED, readerState.dwEventState);
			}

			if (readerState.dwEventState & SCARD_STATE_EMPTY)
			{
				debugInfo2(__func__, " �J�[�h���Z�b�g����Ă��Ȃ�\n");
				return false;
			}

			if (readerState.dwEventState & SCARD_STATE_INUSE && (hCard == NULL)) //�������g���Ă�Ƃ��͂����g�p��
			{
				debugInfo2(__func__, " �J�[�h�͑��̃A�v���P�[�V�����Ɏg�p����Ă��܂�\n");
				return false;
			}

			if (!(readerState.dwEventState & SCARD_STATE_PRESENT))
			{
				debugInfo2(__func__, " �s���ȏ��\n");
				throw PCSCFatalException("�s���ȏ��", PCSCFatalException::UNKNOWN_STATUS, readerState.dwEventState);
			}
			/*
						//����polling����ꍇ�C�J�[�h���Z�b�g����Ă��Ȃ��̂ɃZ�b�g����Ă���悤�ɂ݂��邱�Ƃ�����΍�
						if ( (hCard != NULL) && (isFelica()) ) //�ڑ����ł���Felica�Ȃ�
						{
							debugInfo2(__func__, " �V�X�e���R�[�h�X�L���������s\n");
							try
							{
								cardSystemcode = readSystemcode();
							} catch ( PCSCFatalException e )
							{
								//���s�����Ȃ�΃J�[�h���Ȃ�
								debugInfo2(__func__, " �J�[�h�����O���ꂽ\n");
								Sleep(2000);//����
								return false;
							}
						}
			*/
			debugInfo2(__func__, " �J�[�h���Z�b�g����Ă��܂� %X\n", readerState.dwEventState);
			return true;
		}

		uint32_t getCurrentState()
		{
			return readerState.dwCurrentState;
		}

		//�J�[�h���Z�b�g�����܂Ŏw�莞�ԑ҂DINFINYTE���Z�b�g����Ɩ����ɑ҂�
		bool waitForSetCard(uint32_t timeout = INFINITE)
		{
			checkReadyContext(); //�R���e�L�X�g�����p�\�`�F�b�N(�_���Ȃ��O)

								 //���łɃJ�[�h���Z�b�g����Ă��邩���`�F�b�N�D�����Ɍ��݂̏�Ԃ��X�V
			if (isCardSet())
			{
				return true;
			}

			debugInfo2(__func__, " �J�[�h���Z�b�g�����܂ő҂��܂�\n");
			//��ԕω�����܂ŉi���ɑҋ@
			return isCardSet(readerState.dwCurrentState, timeout);
		}

		//��������Ă���J�[�h�ɐڑ�����
		void connectCard(bool exclusive = true)
		{
			checkReadyContext(); //�R���e�L�X�g�����p�\�`�F�b�N(�_���Ȃ��O)
			disconnectCard(); //���d�ڑ��h�~�̈אؒf���Ă���

			DWORD dwActiveProtocol;
			//�J�[�h�����L���Ȃ�

			DWORD dwShareMode = SCARD_SHARE_SHARED;
			if (exclusive)
			{
				dwShareMode = SCARD_SHARE_EXCLUSIVE;
			}

			//lpszReaderName;
			LONG res = SCardConnectA(hContext, activeReaderName.c_str(), dwShareMode, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard, &dwActiveProtocol);
			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " �J�[�h���Z�b�g����Ă��܂���\n");
					throw PCSCCardRemovedException("�J�[�h���Z�b�g����Ă��܂���");
				}
				else if (res == SCARD_E_SHARING_VIOLATION)
				{
					debugInfo2(__func__, " ���̃A�v���P�[�V�������J�[�h��r���I�Ɏg�p���ł�\n");
					throw PCSCFatalException("���̃A�v���P�[�V�������J�[�h��r���I�Ɏg�p���ł�", PCSCFatalException::CARD_HAS_LOCKED_BY_ANOTHER_PROGRAM, res);
				}
				else
				{
					debugInfo2(__func__, " �s���ȃG���[\n");
					throw PCSCFatalException("�s���ȃG���[", PCSCFatalException::UNKNOWN_ERROR, res);
				}
			}

			//�V�X�e���R�[�h�̎擾�͕���p�������̂ōŏ��Ɏ擾���Ă��܂��D
			cardTypecode = CARD_TYPE_UNKNOWN;////������
			cardSystemcode = SYSTEMCODE_ANY; //������

			cardTypecode = readCardTypeCode();
			if (cardTypecode == CARD_TYPE_FELICA) //Felica����Ȃ����̎擾�ł���
			{
				cardSystemcode = readSystemcode();
			}
			debugInfo2(__func__, " �J�[�h�ɐڑ����܂���(�J�[�h���:%02X, �V�X�e���R�[�h:%04X)\n", cardTypecode, cardSystemcode);
		}

		//�J�[�h�̑��݂ɂ�����炸�J�[�h���[�_�[�ɒ��ڐڑ�����
		void connectDirect()
		{
			checkReadyContext(); //�R���e�L�X�g�����p�\�`�F�b�N(�_���Ȃ��O)
			disconnectCard(); //���d�ڑ��h�~�̈אؒf���Ă���

			DWORD dwActiveProtocol;
			LONG res = SCardConnectA(hContext, activeReaderName.c_str(), SCARD_SHARE_DIRECT, 0, &hCard, &dwActiveProtocol);
			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_E_SHARING_VIOLATION)
				{
					debugInfo2(__func__, " ���̃A�v���P�[�V�������J�[�h��r���I�Ɏg�p���ł�\n");
					throw PCSCFatalException("���̃A�v���P�[�V�������J�[�h��r���I�Ɏg�p���ł�", PCSCFatalException::CARD_HAS_LOCKED_BY_ANOTHER_PROGRAM, res);
				}
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " ���ڐڑ��Ɏ��s\n");
					throw PCSCFatalException("���ڐڑ��Ɏ��s", PCSCFatalException::FAILED_TO_DIRECT_CONNECTION, res);
				}
				else
				{
					debugInfo2(__func__, " �s���ȃG���[\n");
					throw PCSCFatalException("�s���ȃG���[", PCSCFatalException::UNKNOWN_ERROR, res);
				}
			}
			debugInfo2(__func__, " ���ڐڑ����܂���\n");
		}

		//�J�[�h����ؒf����
		void disconnectCard()
		{
			if (hCard != NULL)
			{
				debugInfo2(__func__, " �ڑ���ؒf���܂���\n");
				SCardDisconnect(hCard, SCARD_LEAVE_CARD);
			}
			hCard = NULL;
		}

		//�J�[�h�������ǂݎ��
		void readBinary(uint16_t adr, uint8_t dat[16])
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE DataSize = 3; //�㑱�̃f�[�^��(Le�܂߂�)
			BYTE BlockNum = 1;

			BYTE BlockList[3] = { 0 };
			BlockList[0] = 0; //0x00 : [����(1=1byte,0=2byte:1bit] [�A�N�Z�X���[�h:3bit] [SERVICE�R�[�h���X�g����:4bit]
			BlockList[1] = (adr) & 0xFF;
			BlockList[2] = (adr >> 8) & 0xFF;

			BYTE Le = 16 * BlockNum;

			BYTE pbSendBuffer[256] = { APDU_CLA_GENERIC,APDU_INS_READ_BINARY,USE_BLOCKLIST | NO_RFU,BlockNum };

			BYTE pbRecvBuffer[256] = { 0 };
			DWORD pcbRecvLength = 256;

			//�f�[�^����
			int i = 0;
			for (i = 0; i < DataSize; i++)
			{
				pbSendBuffer[5 + i] = BlockList[i];
			}
			pbSendBuffer[4] = i; //�f�[�^���X�g�T�C�Y����������
			pbSendBuffer[5 + i] = Le; //�Ō�Ɏ�M����u���b�N�T�C�Y����������
			int bufsize = i + 6;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, bufsize, NULL, pbRecvBuffer, &pcbRecvLength);
			if (res == SCARD_W_REMOVED_CARD)
			{
				debugInfo2(__func__, " �J�[�h�����O����܂����D\n");
				throw PCSCCardRemovedException("�J�[�h�����O����܂����D");
			}

			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException("�J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];
			if (SW1 != 0x90 || SW2 != 0x00)
			{
				debugInfo2(__func__, " �A�h���X:%04X �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", adr, SW1, SW2);
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (SW1 == 0x91 && SW2 == 0x00)
				{
					debugInfo2(__func__, " Felica�J�[�h�������G���[�ł��D�R�[�h:%02X,%02X\n", pbRecvBuffer[0], pbRecvBuffer[1]);
					debugInfo2(__func__, " %s\n", FellicaErrorInfo(pbRecvBuffer[1]));
					throw FelicaErrorException(FellicaErrorInfo(pbRecvBuffer[1]), pbRecvBuffer[0], pbRecvBuffer[1]);
				}

				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}
			if (pcbRecvLength != (16 + 2))
			{
				debugInfo2(__func__, " �����T�C�Y�ُ� %d\n", pcbRecvLength);
				throw FelicaFatalException("�����T�C�Y�ُ�", FelicaFatalException::INVALID_RESPONSE);
			}

			//�f�[�^�R�s�[
			for (DWORD i = 0; i < pcbRecvLength - 2; i++)
			{
				if (i < 16)
					dat[i] = pbRecvBuffer[i];
			}

			if (debugFlag)
			{
				debugInfo2(__func__, " �A�h���X:%04X �ǂݍ��݊���\n", adr);
				debugInfo2(__func__, " ���e:");
				for (DWORD i = 0; i < pcbRecvLength - 2; i++)
				{
					printf("%02X,", pbRecvBuffer[i]);
				}
				printf("\n");
			}
		}

		//�J�[�h����MAC�t���ŏ���ǂݎ��
		void readBinaryWithMAC(uint16_t adr, uint8_t dat[16], uint8_t mac[16])
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE DataSize = 6; //�㑱�̃f�[�^��(Le�܂߂�)
			BYTE BlockNum = 2;

			BYTE BlockList[6] = { 0 };
			BlockList[0] = 0; //0x00 : [����(1=1byte,0=2byte:1bit] [�A�N�Z�X���[�h:3bit] [SERVICE�R�[�h���X�g����:4bit]
			BlockList[1] = (adr) & 0xFF;
			BlockList[2] = (adr >> 8) & 0xFF;
			BlockList[3] = 0; //0x00 : [����(1=1byte,0=2byte:1bit] [�A�N�Z�X���[�h:3bit] [SERVICE�R�[�h���X�g����:4bit]
			BlockList[4] = ADDRESS_MAC;//MAC
			BlockList[5] = 0x00;

			BYTE Le = 16 * BlockNum;

			BYTE pbSendBuffer[256] = { APDU_CLA_GENERIC,APDU_INS_READ_BINARY,USE_BLOCKLIST | NO_RFU,BlockNum };

			BYTE pbRecvBuffer[256] = { 0 };
			DWORD pcbRecvLength = 256;

			//�f�[�^����
			int i = 0;
			for (i = 0; i < DataSize; i++)
			{
				pbSendBuffer[5 + i] = BlockList[i];
			}
			pbSendBuffer[4] = i; //�f�[�^���X�g�T�C�Y����������
			pbSendBuffer[5 + i] = Le; //�Ō�Ɏ�M����u���b�N�T�C�Y����������
			int bufsize = i + 6;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, bufsize, NULL, pbRecvBuffer, &pcbRecvLength);
			if (res == SCARD_W_REMOVED_CARD)
			{
				debugInfo2(__func__, " �J�[�h�����O����܂����D\n");
				throw PCSCCardRemovedException("�J�[�h�����O����܂����D");
			}

			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException("�J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];
			if (SW1 != 0x90 || SW2 != 0x00)
			{
				debugInfo2(__func__, " �A�h���X:%04X �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", adr, SW1, SW2);
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (SW1 == 0x91 && SW2 == 0x00)
				{
					debugInfo2(__func__, " Felica�J�[�h�������G���[�ł��D�R�[�h:%02X,%02X\n", pbRecvBuffer[0], pbRecvBuffer[1]);
					debugInfo2(__func__, " %s\n", FellicaErrorInfo(pbRecvBuffer[1]));
					throw FelicaErrorException(FellicaErrorInfo(pbRecvBuffer[1]), pbRecvBuffer[0], pbRecvBuffer[1]);
				}

				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			if (pcbRecvLength != (32 + 2))
			{
				debugInfo2(__func__, " �����T�C�Y�ُ� %d\n", pcbRecvLength);
				throw FelicaFatalException("�����T�C�Y�ُ�", FelicaFatalException::INVALID_RESPONSE);
			}

			//�f�[�^�R�s�[
			for (DWORD i = 0; i < pcbRecvLength - 2; i++)
			{
				if (i < 16)
				{
					dat[i] = pbRecvBuffer[i];
					mac[i] = pbRecvBuffer[i + 16];
				}
			}

			if (debugFlag)
			{
				debugInfo2(__func__, " �A�h���X:%04X �ǂݍ��݊���\n", adr);
				debugInfo2(__func__, " ���e:");
				for (DWORD i = 0; i < 16; i++)
				{
					printf("%02X,", dat[i]);
				}
				printf("\n");
				debugInfo2(__func__, " MAC:");
				for (DWORD i = 0; i < 16; i++)
				{
					printf("%02X,", mac[i]);
				}
				printf("\n");
			}
		}

		//�J�[�h����MAC_A�t���ŏ���ǂݎ��
		void readBinaryWithMAC_A(uint16_t adr, uint8_t dat[16], uint8_t mac[16])
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE DataSize = 6; //�㑱�̃f�[�^��(Le�܂߂�)
			BYTE BlockNum = 2;

			BYTE BlockList[6] = { 0 };
			BlockList[0] = 0; //0x00 : [����(1=1byte,0=2byte:1bit] [�A�N�Z�X���[�h:3bit] [SERVICE�R�[�h���X�g����:4bit]
			BlockList[1] = (adr) & 0xFF;
			BlockList[2] = (adr >> 8) & 0xFF;
			BlockList[3] = 0; //0x00 : [����(1=1byte,0=2byte:1bit] [�A�N�Z�X���[�h:3bit] [SERVICE�R�[�h���X�g����:4bit]
			BlockList[4] = ADDRESS_MAC_A;//MAC_A
			BlockList[5] = 0x00;

			BYTE Le = 16 * BlockNum;

			BYTE pbSendBuffer[256] = { APDU_CLA_GENERIC,APDU_INS_READ_BINARY,USE_BLOCKLIST | NO_RFU,BlockNum };

			BYTE pbRecvBuffer[256] = { 0 };
			DWORD pcbRecvLength = 256;

			//�f�[�^����
			int i = 0;
			for (i = 0; i < DataSize; i++)
			{
				pbSendBuffer[5 + i] = BlockList[i];
			}
			pbSendBuffer[4] = i; //�f�[�^���X�g�T�C�Y����������
			pbSendBuffer[5 + i] = Le; //�Ō�Ɏ�M����u���b�N�T�C�Y����������
			int bufsize = i + 6;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, bufsize, NULL, pbRecvBuffer, &pcbRecvLength);
			if (res == SCARD_W_REMOVED_CARD)
			{
				debugInfo2(__func__, " �J�[�h�����O����܂����D\n");
				throw PCSCCardRemovedException("�J�[�h�����O����܂����D");
			}

			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException("�J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];
			if (SW1 != 0x90 || SW2 != 0x00)
			{
				debugInfo2(__func__, " �A�h���X:%04X �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", adr, SW1, SW2);
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (SW1 == 0x91 && SW2 == 0x00)
				{
					debugInfo2(__func__, " Felica�J�[�h�������G���[�ł��D�R�[�h:%02X,%02X\n", pbRecvBuffer[0], pbRecvBuffer[1]);
					debugInfo2(__func__, " %s\n", FellicaErrorInfo(pbRecvBuffer[1]));
					throw FelicaErrorException(FellicaErrorInfo(pbRecvBuffer[1]), pbRecvBuffer[0], pbRecvBuffer[1]);
				}

				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			if (pcbRecvLength != (32 + 2))
			{
				debugInfo2(__func__, " �����T�C�Y�ُ� %d\n", pcbRecvLength);
				throw FelicaFatalException("�����T�C�Y�ُ�", FelicaFatalException::INVALID_RESPONSE);
			}

			//�f�[�^�R�s�[
			for (DWORD i = 0; i < pcbRecvLength - 2; i++)
			{
				if (i < 16)
				{
					dat[i] = pbRecvBuffer[i];
					mac[i] = pbRecvBuffer[i + 16];
				}
			}

			if (debugFlag)
			{
				debugInfo2(__func__, " �A�h���X:%04X �ǂݍ��݊���\n", adr);
				debugInfo2(__func__, " ���e:");
				for (DWORD i = 0; i < 16; i++)
				{
					printf("%02X,", dat[i]);
				}
				printf("\n");
				debugInfo2(__func__, " MAC:");
				for (DWORD i = 0; i < 16; i++)
				{
					printf("%02X,", mac[i]);
				}
				printf("\n");
			}
		}

		//�J�[�h�ւ̏�������
		void updateBinary(uint16_t adr, uint8_t dat[16])
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE DataSize = 3; //�㑱�̃f�[�^��(Le�܂߂�)
			BYTE BlockNum = 1;

			BYTE BlockList[3] = { 0 };
			BlockList[0] = 0; //0x00 : [����(1=1byte,0=2byte:1bit] [�A�N�Z�X���[�h:3bit] [SERVICE�R�[�h���X�g����:4bit]
			BlockList[1] = (adr) & 0xFF;
			BlockList[2] = (adr >> 8) & 0xFF;

			BYTE Le = 0;

			BYTE pbSendBuffer[256] = { APDU_CLA_GENERIC,APDU_INS_UPDATE_BINARY,USE_BLOCKLIST | NO_RFU,BlockNum };

			BYTE pbRecvBuffer[256];
			DWORD pcbRecvLength = 256;

			//�f�[�^����
			int i, j;
			for (i = 0; i < DataSize; i++)
			{
				pbSendBuffer[5 + i] = BlockList[i];
			}
			for (j = 0; j < 16; j++)
			{
				pbSendBuffer[5 + i + j] = dat[j];
			}
			pbSendBuffer[4] = i + j; //�f�[�^�T�C�Y����������
			pbSendBuffer[5 + i + j] = Le; //�Ō�Ɏ�M����u���b�N�T�C�Y����������
			DWORD bufsize = i + j + 6;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), NULL, pbRecvBuffer, &pcbRecvLength);
			if (res == SCARD_W_REMOVED_CARD)
			{
				debugInfo2(__func__, " �J�[�h�����O����܂����D\n");
				throw PCSCCardRemovedException("�J�[�h�����O����܂����D");
			}

			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException(" �J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//���X�|���X���
			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				debugInfo2(__func__, " �A�h���X:%04X �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", adr, SW1, SW2);
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}

				if (SW1 == 0x91 && SW2 == 0x00)
				{
					if (pbRecvBuffer[0] == 0xFF && pbRecvBuffer[1] == 0x71)
					{
						debugInfo2(__func__, " Felica�J�[�h�������x���ł�(�����������߂�)�R�[�h:%02X,%02X\n", pbRecvBuffer[0], pbRecvBuffer[1]);
					}
					else {
						debugInfo2(__func__, " Felica�J�[�h�������G���[�ł��D�R�[�h:%02X,%02X\n", pbRecvBuffer[0], pbRecvBuffer[1]);
						debugInfo2(__func__, " %s\n", FellicaErrorInfo(pbRecvBuffer[1]));
						throw FelicaErrorException(FellicaErrorInfo(pbRecvBuffer[1]), pbRecvBuffer[0], pbRecvBuffer[1]);
					}
				}

				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);

			}
			if (debugFlag)
			{
				debugInfo2(__func__, "  �A�h���X:%04X �������ݐ���\n", adr);
				debugInfo2(__func__, " ���e:");
				for (DWORD i = 0; i < 16; i++)
				{
					printf("%02X,", dat[i]);
				}
				printf("\n");
			}
		}
		//�J�[�h�ւ̏�������
		void updateBinaryWithMAC_A(uint16_t adr, uint8_t dat[16], uint8_t mac[16])
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE DataSize = 6; //�㑱�̃f�[�^��(Le�܂߂�)
			BYTE BlockNum = 2;

			BYTE BlockList[6] = { 0 };
			BlockList[0] = 0; //0x00 : [����(1=1byte,0=2byte:1bit] [�A�N�Z�X���[�h:3bit] [SERVICE�R�[�h���X�g����:4bit]
			BlockList[1] = (adr) & 0xFF;
			BlockList[2] = (adr >> 8) & 0xFF;
			BlockList[3] = 0; //0x00 : [����(1=1byte,0=2byte:1bit] [�A�N�Z�X���[�h:3bit] [SERVICE�R�[�h���X�g����:4bit]
			BlockList[4] = ADDRESS_MAC_A;//MAC_A
			BlockList[5] = 0x00;

			BYTE Le = 0;

			BYTE pbSendBuffer[256] = { APDU_CLA_GENERIC,APDU_INS_UPDATE_BINARY,USE_BLOCKLIST | NO_RFU,BlockNum };

			BYTE pbRecvBuffer[256];
			DWORD pcbRecvLength = 256;

			//�f�[�^����
			int i, j, k;
			for (i = 0; i < DataSize; i++)
			{
				pbSendBuffer[5 + i] = BlockList[i];
			}
			for (j = 0; j < 16; j++)
			{
				pbSendBuffer[5 + i + j] = dat[j];
			}
			for (k = 0; k < 16; k++)
			{
				pbSendBuffer[5 + i + j + k] = mac[k];
			}
			pbSendBuffer[4] = i + j + k; //�f�[�^�T�C�Y����������
			pbSendBuffer[5 + i + j + k] = Le; //�Ō�Ɏ�M����u���b�N�T�C�Y����������
			DWORD bufsize = i + j + k + 6;


			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), NULL, pbRecvBuffer, &pcbRecvLength);
			if (res == SCARD_W_REMOVED_CARD)
			{
				debugInfo2(__func__, " �J�[�h�����O����܂����D\n");
				throw PCSCCardRemovedException("�J�[�h�����O����܂����D");
			}

			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException(" �J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//���X�|���X���
			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				debugInfo2(__func__, " �A�h���X:%04X �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", adr, SW1, SW2);
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}

				if (SW1 == 0x91 && SW2 == 0x00)
				{
					if (pbRecvBuffer[0] == 0xFF && pbRecvBuffer[1] == 0x71)
					{
						debugInfo2(__func__, " Felica�J�[�h�������x���ł�(�����������߂�)�R�[�h:%02X,%02X\n", pbRecvBuffer[0], pbRecvBuffer[1]);
					}
					else
					{
						debugInfo2(__func__, " Felica�J�[�h�������G���[�ł��D�R�[�h:%02X,%02X\n", pbRecvBuffer[0], pbRecvBuffer[1]);
						debugInfo2(__func__, " %s\n", FellicaErrorInfo(pbRecvBuffer[1]));
						throw FelicaErrorException(FellicaErrorInfo(pbRecvBuffer[1]), pbRecvBuffer[0], pbRecvBuffer[1]);
					}
				}

				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);

			}
			if (debugFlag)
			{
				debugInfo2(__func__, "  �A�h���X:%04X �������ݐ���\n", adr);
				debugInfo2(__func__, " ���e:");
				for (DWORD i = 0; i < 16; i++)
				{
					printf("%02X,", dat[i]);
				}
				printf("\n");
				debugInfo2(__func__, " MAC:");
				for (DWORD i = 0; i < 16; i++)
				{
					printf("%02X,", mac[i]);
				}
				printf("\n");
			}

		}

		//------Felica Lite�֌W����------

		//PaSoRi�̃V���A���i���o�[���擾(�J�[�h���C���邢�͒��ڐڑ�����K�v����)
		void getPaSoRiSerialNumber(char* serialNumberString = NULL)
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE lpInBuffer[] = { ESC_CMD_GET_INFO,PRODUCT_SERIAL_NUMBER };
			BYTE lpOutBuffer[256];
			DWORD lpBytesReturned = 0;

			LONG res = SCardControl(hCard, SCARD_CTL_CODE(3500), lpInBuffer, sizeof(lpInBuffer), lpOutBuffer, sizeof(lpOutBuffer), &lpBytesReturned);
			if (res != SCARD_S_SUCCESS)
			{
				debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//Debug�p
			if (serialNumberString != NULL)
			{
				for (int i = 0; i < 256; i++)
					serialNumberString[i] = lpOutBuffer[i];
			}

			debugInfo2(__func__, " �J�[�h���[�_�[�V���A���i���o�[:%s\n", lpOutBuffer);
		}

		//�J�[�h��IDm���擾
		uint32_t readUID(uint8_t UID[] = NULL)
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

							  //CLA,INS,P1,P2,Le
			BYTE pbSendBuffer[5] = { APDU_CLA_GENERIC, APDU_INS_GET_DATA, APDU_P1_GET_UID,APDU_P2_NONE,APDU_LE_MAX_LENGTH };

			BYTE pbRecvBuffer[256];
			DWORD pcbRecvLength = 256;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), NULL, pbRecvBuffer, &pcbRecvLength);
			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException("�J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//���X�|���X���
			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}

				debugInfo2(__func__, " �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", SW1, SW2);
				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			if (debugFlag)
			{
				debugInfo2(__func__, " UID:");
				for (DWORD i = 0; i < pcbRecvLength - 2; i++) //�Ō��2�o�C�g�̓��X�|���X
				{
					printf("%02X", pbRecvBuffer[i]);
				}
				printf("\n");
			}

			if (UID != NULL)
			{
				for (DWORD i = 0; i < pcbRecvLength - 2; i++) //�Ō��2�o�C�g�̓��X�|���X
				{
					UID[i] = pbRecvBuffer[i];
				}
			}
			return (uint32_t)(pcbRecvLength - 2);
		}
		//�J�[�h��PMm���擾
		uint32_t readPMm(uint8_t PMm[] = NULL)
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

							  //CLA,INS,P1,P2,Le
			BYTE pbSendBuffer[5] = { APDU_CLA_GENERIC, APDU_INS_GET_DATA, APDU_P1_GET_PMm,APDU_P2_NONE,APDU_LE_MAX_LENGTH };

			BYTE pbRecvBuffer[256];
			DWORD pcbRecvLength = 256;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), NULL, pbRecvBuffer, &pcbRecvLength);
			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException("�J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//���X�|���X���
			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}

				debugInfo2(__func__, " �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", SW1, SW2);
				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			if (debugFlag)
			{
				debugInfo2(__func__, " PMm:");
				for (DWORD i = 0; i < pcbRecvLength - 2; i++) //�Ō��2�o�C�g�̓��X�|���X
				{
					printf("%02X", pbRecvBuffer[i]);
				}
				printf("\n");
			}

			if (PMm != NULL)
			{
				for (DWORD i = 0; i < pcbRecvLength - 2; i++) //�Ō��2�o�C�g�̓��X�|���X
				{
					PMm[i] = pbRecvBuffer[i];
				}
			}
			return (uint32_t)(pcbRecvLength - 2);
		}
		//�J�[�h�̎�ނ��擾
		uint8_t getCardTypeCode()
		{
			return cardTypecode;
		}

		//�J�[�h�̎�ނ��擾
		uint8_t readCardTypeCode()
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

							  //CLA,INS,P1,P2,Le
			BYTE pbSendBuffer[5] = { APDU_CLA_GENERIC, APDU_INS_GET_DATA, APDU_P1_GET_CARD_TYPE,APDU_P2_NONE,APDU_LE_MAX_LENGTH };

			BYTE pbRecvBuffer[256];
			DWORD pcbRecvLength = 256;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), NULL, pbRecvBuffer, &pcbRecvLength);
			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException("�J�[�h�n���h���������ɂȂ�܂����D");
				}

				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}

				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}


			//���X�|���X���
			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}

				debugInfo2(__func__, " �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", SW1, SW2);
				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			if (debugFlag)
			{
				debugInfo2(__func__, " �J�[�h���:%02X\n", pbRecvBuffer[0]);
				switch (pbRecvBuffer[0])
				{
				case CARD_TYPE_FELICA: debugInfo2(__func__, " ���̃J�[�h��Felica�ł��D\n"); break;
				case CARD_TYPE_ISO14443A: debugInfo2(__func__, " ���̃J�[�h��ISO14443A�ł��D\n"); break;
				case CARD_TYPE_ISO14443B: debugInfo2(__func__, " ���̃J�[�h��ISO14443B�ł��D\n"); break;
				case CARD_TYPE_PICOPASSB: debugInfo2(__func__, " ���̃J�[�h��PICOPASSB�ł��D\n"); break;
				case CARD_TYPE_NFC_TYPE_1: debugInfo2(__func__, " ���̃J�[�h��NFC_TYPE_1�ł��D\n"); break;
				case CARD_TYPE_MIFARE_EC: debugInfo2(__func__, " ���̃J�[�h��MIFARE_EC�ł��D\n"); break;
				case CARD_TYPE_ISO14443A_4A: debugInfo2(__func__, " ���̃J�[�h��ISO14443A_4A�ł��D\n"); break;
				case CARD_TYPE_ISO14443B_4B: debugInfo2(__func__, " ���̃J�[�h��ISO14443B_4B�ł��D\n"); break;
				case CARD_TYPE_TYPE_A_NFC_DEP: debugInfo2(__func__, " ���̃J�[�h��TYPE_A_NFC_DEP�ł��D\n"); break;
				case CARD_TYPE_FELICA_NFC_DEP: debugInfo2(__func__, " ���̃J�[�h��FELICA_NFC_DEP�ł��D\n"); break;
				default: debugInfo2(__func__, " ���̃J�[�h�̎�ނ͕s���ł��D\n"); break;
				}
			}

			return pbRecvBuffer[0];
		}


		bool isFelica()
		{
			return (getCardTypeCode() == CARD_TYPE_FELICA);
		}
		bool isFelicaLite()
		{
			if (!isFelica())
			{
				debugInfo2(__func__, " ���̃J�[�h��Felica�ł͂Ȃ�\n");
				return false;
			}

			if ((getSystemcode() != SYSTEMCODE_NFC_TYPE3) && (getSystemcode() != SYSTEMCODE_FELICALITE))
			{
				debugInfo2(__func__, " ���̃J�[�h��Felica Lite�ł͂Ȃ�\n");
				return false;
			}
			debugInfo2(__func__, " ���̃J�[�h��Felica Lite�n�ł�\n");
			return true;
		}

		/*
		bool isFelicaLite()
		{
		if(getCardTypeCode() != CARD_TYPE_FELICA)
		{
		debugInfo2(__func__," ���̃J�[�h��Felica�ł͂Ȃ�(�J�[�h��ʂ��Ⴄ)\n");
		return false;
		}

		uint8_t PMm[64]={0};
		uint32_t  size = readPMm(PMm);
		if(size != 8)
		{
		debugInfo2(__func__," ���̃J�[�h��Felica�ł͂Ȃ�(PMm��8byte�ł͂Ȃ�)\n");
		return false;
		}

		if(PMm[1] < 0xF0)
		{
		debugInfo2(__func__," ���̃J�[�h��Felica Lite�ł͂Ȃ�(Standard����у��o�C��Felica�Ȃ�)\n");
		return false;
		}

		switch( PMm[1] )
		{
		case 0xF0: debugInfo2(__func__," ���̃J�[�h��Felica Lite�ł��D(RC-S965)\n");break;
		case 0xF1: debugInfo2(__func__," ���̃J�[�h��Felica Lite-S�ł��D(RC-S966)\n"); break;
		case 0xF2: debugInfo2(__func__," ���̃J�[�h��Felica Link�ł��D(RC-S967/Lite-S/Lite-S HT)\n"); break;
		case 0xE0: debugInfo2(__func__," ���̃J�[�h��Felica Plug�ł��D(RC-S926)\n"); break;
		case 0xE1: debugInfo2(__func__," ���̃J�[�h��Felica Link�ł��D(RC-S967/Plug)\n"); break;
		case 0xFF: debugInfo2(__func__," ���̃J�[�h��NFC�d�l��Felica�ł��D\n"); break;
		default :  debugInfo2(__func__," ���̃J�[�h��Felica Lite���ۂ��ł��D(�s��)\n"); break;
		}
		return true;
		}

		bool isFelicaLiteS()
		{
		if ( getCardTypeCode() != CARD_TYPE_FELICA )
		{
		debugInfo2(__func__," ���̃J�[�h��Felica�ł͂Ȃ�(�J�[�h��ʂ��Ⴄ)\n");
		return false;
		}

		uint8_t PMm[64] = {0};
		uint32_t  size = readPMm(PMm);
		if ( size != 8 )
		{
		debugInfo2(__func__," ���̃J�[�h��Felica�ł͂Ȃ�(PMm��8byte�ł͂Ȃ�)\n");
		return false;
		}

		if ( PMm[1] < 0xF0 )
		{
		debugInfo2(__func__," ���̃J�[�h��Felica Lite�ł͂Ȃ�(Standard����у��o�C��Felica�Ȃ�)\n");
		return false;
		}

		if(PMm[1] == 0xF1 )
		{
		debugInfo2(__func__," ���̃J�[�h��Felica Lite-S�ł��D(RC-S966)\n");
		return true;
		}else{
		debugInfo2(__func__," ���̃J�[�h��Felica Lite�ł���Lite-S�ł͂���܂���D\n");
		return false;
		}
		}
		*/
		void readCardTypeString(char* cardTypeString = NULL)
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

							  //CLA,INS,P1,P2,Le
			BYTE pbSendBuffer[5] = { APDU_CLA_GENERIC, APDU_INS_GET_DATA, APDU_P1_GET_CARD_TYPE_NAME,APDU_P2_NONE,APDU_LE_MAX_LENGTH };

			BYTE pbRecvBuffer[256];
			DWORD pcbRecvLength = 256;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), NULL, pbRecvBuffer, &pcbRecvLength);
			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException("�J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//���X�|���X���
			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				debugInfo2(__func__, " �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", SW1, SW2);
				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			for (DWORD i = 0; i < pcbRecvLength - 2; i++) //�Ō��2�o�C�g�̓��X�|���X
			{
				cardTypeString[i] = pbRecvBuffer[i];
			}

			if (debugFlag)
			{
				debugInfo2(__func__, " �J�[�h��ʖ���:");
				for (DWORD i = 0; i < pcbRecvLength - 2; i++) //�Ō��2�o�C�g�̓��X�|���X
				{
					printf("%c", pbRecvBuffer[i]);
				}
				printf("\n");
			}
		}

		//�V�X�e���R�[�h���擾����
		uint16_t getSystemcode()
		{
			return cardSystemcode;
		}

		//����Polling���邱�ƂŃV�X�e���R�[�h���擾����(IDm�Ȃǂ��擾�ł��Ă��邪)
		uint16_t readSystemcode(uint16_t scancode = SYSTEMCODE_ANY)
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE scancode_high = (scancode >> 8) & 0xFF;
			BYTE scancode_low = (scancode) & 0xFF;

			//CLA,INS,P1,P2,Le
			BYTE pbSendBuffer[10] = { APDU_CLA_GENERIC, APDU_INS_DATA_EXCHANGE, APDU_P1_THRU,APDU_P2_TIMEOUT_50MS,EXCHANGE_POLLING_PACKET_SIZE,EXCHANGE_POLLING,scancode_high,scancode_low,POLLING_REQUEST_SYSTEM_CODE,POLLING_TIMESLOT_16 };

			BYTE pbRecvBuffer[256];
			DWORD pcbRecvLength = 256;

			//�R�}���h���M
			LONG res = SCardTransmit(hCard, SCARD_PCI_T1, pbSendBuffer, sizeof(pbSendBuffer), NULL, pbRecvBuffer, &pcbRecvLength);
			if (res != SCARD_S_SUCCESS)
			{
				if (res == SCARD_W_REMOVED_CARD)
				{
					debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
					throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
				}
				if (res == 0x458)
				{
					debugInfo2(__func__, " �J�[�h�n���h���������ɂȂ�܂����D\n");
					throw PCSCCardRemovedException("�J�[�h�n���h���������ɂȂ�܂����D");
				}
				if (res == ERROR_BAD_COMMAND)
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� (���ڐڑ����Ă��܂��񂩁H)%X\n", res);
				}
				else
				{
					debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				}
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//���X�|���X���
			BYTE SW1 = pbRecvBuffer[pcbRecvLength - 2];
			BYTE SW2 = pbRecvBuffer[pcbRecvLength - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					if (scancode == SYSTEMCODE_ANY)
					{
						debugInfo2(__func__, " PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D\n");
						throw PCSCCardRemovedException("PC/SC�R�}���h���s���ɃJ�[�h�����O����܂����D");
					}

					debugInfo2(__func__, " �Y������V�X�e���R�[�h�̃J�[�h�͌�����܂���ł����D(�^�C���A�E�g)\n");
					throw PCSCCardRemovedException("�Y������V�X�e���R�[�h�̃J�[�h�͌�����܂���ł����D(�^�C���A�E�g)");
				}
				debugInfo2(__func__, " �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", SW1, SW2);
				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			if (pcbRecvLength - 2 != 19 || pbRecvBuffer[0] != 0x01)
			{
				debugInfo2(__func__, " ���҂���鉞���ł͂Ȃ��D����%d (�{��:19) ����:%02X\n", pcbRecvLength - 2, pbRecvBuffer[0]);
				throw FelicaFatalException("���҂���鉞���ł͂Ȃ�", FelicaFatalException::INVALID_RESPONSE);
			}

			uint16_t systemcode = ((pbRecvBuffer[pcbRecvLength - 4] << 8) & 0xFF00) | (pbRecvBuffer[pcbRecvLength - 3] & 0xFF);
			debugInfo2(__func__, " �V�X�e���R�[�h:%04X\n", systemcode);

			return systemcode;
		}

		//����Polling����IDm��PMm�ƃV�X�e���R�[�h���擾����(���ڐڑ��p)
		uint16_t polling(uint8_t UID[] = NULL, uint8_t PMm[] = NULL, uint16_t scancode = SYSTEMCODE_ANY)
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE scancode_high = (scancode >> 8) & 0xFF;
			BYTE scancode_low = (scancode) & 0xFF;

			BYTE timeout = 5;//x10ms


			BYTE lpInBuffer[10] = { ESC_CMD_APDU_WRAP, APDU_INS_DATA_EXCHANGE, APDU_P1_THRU,APDU_P2_TIMEOUT_50MS,EXCHANGE_POLLING_PACKET_SIZE,EXCHANGE_POLLING,scancode_high,scancode_low,POLLING_REQUEST_SYSTEM_CODE,POLLING_TIMESLOT_16 };
			BYTE lpOutBuffer[256];
			DWORD lpBytesReturned = 0;

			LONG res = SCardControl(hCard, SCARD_CTL_CODE(3500), lpInBuffer, sizeof(lpInBuffer), lpOutBuffer, sizeof(lpOutBuffer), &lpBytesReturned);
			if (res != SCARD_S_SUCCESS)
			{
				debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//���X�|���X���
			BYTE SW1 = lpOutBuffer[lpBytesReturned - 2];
			BYTE SW2 = lpOutBuffer[lpBytesReturned - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				if (SW1 == 0x63 && SW2 == 0x00)
				{
					debugInfo2(__func__, " �Y������V�X�e���R�[�h�̃J�[�h�͌�����܂���ł����D(�^�C���A�E�g)\n");
					throw PCSCCardRemovedException("�Y������V�X�e���R�[�h�̃J�[�h�͌�����܂���ł����D(�^�C���A�E�g)");
				}
				debugInfo2(__func__, " �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", SW1, SW2);
				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			if (lpBytesReturned - 2 != 19 || lpOutBuffer[0] != 0x01)
			{
				debugInfo2(__func__, " ���҂���鉞���ł͂Ȃ��D����%d (�{��:19) ����:%02X\n", lpBytesReturned - 2, lpOutBuffer[0]);
				throw FelicaFatalException("���҂���鉞���ł͂Ȃ�", FelicaFatalException::INVALID_RESPONSE);
			}

			if (UID != NULL)
			{
				for (DWORD i = 0; i < 8; i++)
				{
					UID[i] = lpOutBuffer[i + 1];
				}
			}

			if (PMm != NULL)
			{
				for (DWORD i = 0; i < 8; i++)
				{
					PMm[i] = lpOutBuffer[i + 9];
				}
			}

			uint16_t systemcode = ((lpOutBuffer[lpBytesReturned - 4] << 8) & 0xFF00) | (lpOutBuffer[lpBytesReturned - 3] & 0xFF);

			if (debugFlag)
			{
				debugInfo2(__func__, " IDm:");
				for (DWORD i = 0; i < 8; i++)
				{
					printf("%02X", lpOutBuffer[i + 1]);
				}
				printf("\n");
				debugInfo2(__func__, " PMm:");
				for (DWORD i = 0; i < 8; i++)
				{
					printf("%02X", lpOutBuffer[i + 9]);
				}
				printf("\n");
				debugInfo2(__func__, " �V�X�e���R�[�h:%04X\n", systemcode);
			}

			return systemcode;
		}

		//���ڃR�}���h�𓊂���(���C�u�����J���p�B���֐�)
		void sendControl()
		{
			checkReadyCard(); //�J�[�h�����p�\�`�F�b�N

			BYTE lpInBuffer[] = { 0xFF,0x00,0x00,0x00,0x03,0xD4,0x12,0x34 };
			BYTE lpOutBuffer[256];
			DWORD lpBytesReturned = 0;

			LONG res = SCardControl(hCard, SCARD_CTL_CODE(3500), lpInBuffer, sizeof(lpInBuffer), lpOutBuffer, sizeof(lpOutBuffer), &lpBytesReturned);
			if (res != SCARD_S_SUCCESS)
			{
				debugInfo2(__func__, " �R�}���h�Ɍ�肪���邩�s�K�؂ȏ�Ԃł� %X\n", res);
				throw PCSCCommandException("�R�}���h�Ɍ�肪���邩�s�K�؂ȏ��", res);
			}

			//���X�|���X���
			BYTE SW1 = lpOutBuffer[lpBytesReturned - 2];
			BYTE SW2 = lpOutBuffer[lpBytesReturned - 1];

			if (SW1 != 0x90 || SW2 != 0x00)
			{
				debugInfo2(__func__, " �J�[�h�̓G���[�������܂����DSW1=%02X,SW2=%02X\n", SW1, SW2);
				throw PCSCErrorException("PC/SC �J�[�h�G���[����", SW1, SW2);
			}

			for (DWORD i = 0; i < lpBytesReturned - 2; i++) //�Ō��2�o�C�g�̓��X�|���X
			{
				printf("%02X ", lpOutBuffer[i]);
			}
			printf("\n");

			return;
		}



		//NDEF���L����
		bool isNdefEnabled()
		{
			if (!isFelicaLite())
			{
				throw FelicaFatalException("���̃J�[�h��Felica Lite�ł͂Ȃ�", FelicaFatalException::CARD_IS_NOT_FELICA_LITE);
			}

			uint8_t mc[16] = { 0 };
			readBinary(ADDRESS_MC, mc);
			if (mc[3] == 0x01)
			{
				debugInfo2(__func__, " ���̃J�[�h��NDEF���L���ł�\n");
				return true;
			}
			else
			{
				debugInfo2(__func__, " ���̃J�[�h��NDEF�������ł�\n");
				return false;
			}
		}

		//1�����s�ς�
		bool isFirstIssued()
		{
			if (!isFelicaLite())
			{
				throw FelicaFatalException("���̃J�[�h��Felica Lite�ł͂Ȃ�", FelicaFatalException::CARD_IS_NOT_FELICA_LITE);
			}

			uint8_t mc[16] = { 0 };
			readBinary(ADDRESS_MC, mc);
			if (mc[2] != 0xFF)
			{
				debugInfo2(__func__, " ���̃J�[�h��1�����s�ς݂ł�\n");
				return true;
			}
			else
			{
				debugInfo2(__func__, " ���̃J�[�h�͂܂�1�����s����Ă��܂���\n");
				return false;
			}
		}

		//2�����s�ς�
		bool isSecondIssued()
		{
			if (!isFelicaLite())
			{
				throw FelicaFatalException("���̃J�[�h��Felica Lite�ł͂Ȃ�", FelicaFatalException::CARD_IS_NOT_FELICA_LITE);
			}

			uint8_t mc[16] = { 0 };
			readBinary(ADDRESS_MC, mc);
			if ((mc[1] & 0x80) == 0)
			{
				debugInfo2(__func__, " ���̃J�[�h��2�����s�ς݂ł�\n");
				return true;
			}
			else
			{
				debugInfo2(__func__, " ���̃J�[�h�͂܂�2�����s����Ă��܂���\n");
				return false;
			}
		}

		//------�I�}�P�@�\----

		//�ȈՓI��1�����s���s��(��x�Ɩ߂��Ȃ��̂Œ��ӁI)
		void FirstIssue()
		{
			if (!isFelicaLite())
			{
				throw FelicaFatalException("Felica Lite�ł͂Ȃ��J�[�h�����������邱�Ƃ͂ł��܂���", FelicaFatalException::CARD_IS_NOT_FELICA_LITE);
			}

			if (isFirstIssued())
			{
				throw FelicaFatalException("1�����s�ς̂��ߏ��������s�\", FelicaFatalException::ALREADY_FIRST_ISSUED);
			}
			debugInfo2(__func__, " 1�����s���s���܂�\n");

			//MC_ALL = RO
			//SYS_OP = NDEF�Ή�
			//MC_CKCKV_W_MAC_A = 1 (�������݂�MAC�K�v/���Ƃ���J�[�h�������������� )
			//MC_STATE_W_MAC_A = 1 (���ݔF�ؗL��)

			uint8_t mc[16] = { 0xFF,0xFF,0x00,0x01,0x07,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00 };
			updateBinary(ADDRESS_MC, mc);

			debugInfo2(__func__, " 1�����s���s���܂���\n");
		}

		//NDEF��L���E�����ɂ���D1�����s�O�̂݉\�D1�����s�͂��Ȃ�����0�����s+NDEF�ƂȂ�
		void enableNDEF(bool en = true)
		{
			if (!isFelicaLite())
			{
				throw FelicaFatalException("Felica Lite�ł͂Ȃ��J�[�h�����������邱�Ƃ͂ł��܂���", FelicaFatalException::CARD_IS_NOT_FELICA_LITE);
			}
			if (isFirstIssued())
			{
				throw FelicaFatalException("1�����s�ς̂��ߏ��������s�\", FelicaFatalException::ALREADY_FIRST_ISSUED);
			}

			uint8_t mc[16] = { 0 };
			readBinary(ADDRESS_MC, mc);
			if (en)
			{
				debugInfo2(__func__, " NDEF�L����\n");
				mc[3] = 0x01;
			}
			else
			{
				debugInfo2(__func__, " NDEF������\n");
				mc[3] = 0x00;
			}
			updateBinary(ADDRESS_MC, mc);
		}

		//�ȈՓI��NDEF(URI)����������
		void writeNdefURI(uint8_t mode, const char str[])
		{
			//mode
			//0x01- http://www.
			//0x02->https://www.
			//0x03->http://
			//0x04->https://

			if (!isFelicaLite())
			{
				throw FelicaFatalException("Felica Lite�ł͂Ȃ��J�[�h�����������邱�Ƃ͂ł��܂���", FelicaFatalException::CARD_IS_NOT_FELICA_LITE);
			}

			if (str == NULL)
			{
				throw std::invalid_argument("NullPointerException");
			}

			uint8_t ndef_head[16] = { 0x10,0x04,0x01,0x00,0x0D,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00 };
			uint8_t ndef_urlhead[16] = { 0xD1,0x02,0xC2,0x53,0x70,0x91,0x01,0x03,0x54,0x02,0x6A,0x61,0x11,0x01,0x00,0x55 };
			uint8_t ndef_foot[6] = { 0x51,0x03,0x01,0x61,0x63,0x74 };
			uint8_t url_buf[256] = { mode };

			int len = 0;
			for (len = 0; len < 184; len++)
			{
				url_buf[len + 1] = str[len] & 0xFF;
				if (str[len] == '\0')
					break;
			}
			if (len >= 184)
			{
				throw std::length_error("URI����������");
			}

			for (int i = 0; i < 6; i++)
			{
				url_buf[len + i + 1] = ndef_foot[i];
			}

			ndef_urlhead[14] = len + 1;
			ndef_urlhead[2] = ndef_urlhead[14] + 18;

			//Checksum�v�Z
			ndef_head[13] = ndef_urlhead[14] + 23;

			int sum = 0;
			for (int i = 0; i < 16; i++)
			{
				sum += ndef_head[i];
			}
			ndef_head[15] = sum;

			updateBinary(ADDRESS_SPAD0, ndef_head);
			updateBinary(ADDRESS_SPAD1, ndef_urlhead);

			for (int i = 0; i < 12; i++)
			{
				unsigned char buf[16] = { 0 };
				for (int k = 0; k < 16; k++)
				{
					buf[k] = url_buf[(i * 16) + k];
				}

				updateBinary(ADDRESS_SPAD2 + i, buf);
			}

			debugInfo2(__func__, " NDEF�������݊��� %s\n", str);
		}

		const char* FellicaErrorInfo(uint8_t code)
		{
			switch (code)
			{
			case 0x01: return ("�p�[�X�A���_�[�t���[");
			case 0x02: return ("�L���b�V���o�b�N�I�[�o�[�t���[");
			case 0x70: return ("�v���I�������G���[");
			case 0x71: return ("�����������x��(�������p���ł��܂�)");
			case 0xA1: return ("�T�[�r�X����0x01�łȂ�");
			case 0xA2: return ("�u���b�N����0x01�`0x04�͈̔͊O");
			case 0xA3: return ("�u���b�N���X�g�̃T�[�r�X�R�[�h���X�g���Ԃ�0�łȂ�");
			case 0xA4: return ("�T�[�r�X�^�C�v�s��");
			case 0xA5: return ("�A�N�Z�X�s����");
			case 0xA6: return ("�T�[�r�X�R�[�h��RW�܂���RO�ł͂Ȃ����C�s��v");
			case 0xA7: return ("�u���b�N���X�g�̃A�N�Z�X���[�h��0�łȂ�");
			case 0xA8: return ("�u���b�N�ԍ�������ł͂Ȃ����CRO�̈��RW�œǂݏo�����Ƃ��Ă���C���邢��D2��0�łȂ�");
			case 0xA9: return ("�������ݎ��s");
			case 0xB0: return ("MAC��MAC_A�u���b�N�𓯎��ɃA�N�Z�X���Ă���");
			case 0xB1: return ("�F�؂��K�v�ȃu���b�N�ŔF�؂��Ă��Ȃ�");
			case 0xB2: return ("MAC_A�s��v(RC�������݂Ȃ��CWCNT�s��v�A�������݃f�[�^�s��v�A�������݉񐔃G���[�Ȃ�)���邢��MAC�t���������ݕs�v�ȃu���b�N�ɏ����������Ƃ��Ă���");
			default: return ("�s���ȃG���[");
			}
		}

		//-----------------------------------------

		//8�o�C�g�Œ蒷��DES�Í������s��
		void desEncryptFixedLength(const uint8_t input[8], const uint8_t key[8], const uint8_t IV[8], uint8_t output[8])
		{
			const int dataLength = 8;
			DWORD KeyObjectSize;
			BCRYPT_ALG_HANDLE hAlgorithm;
			DWORD res = 0;

			if (BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_DES_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0))
			{
				throw PCSCCryptographicException("BCryptOpenAlgorithmProvider���G���[��Ԃ��܂���");
			}
			if (BCryptGetProperty(hAlgorithm, BCRYPT_OBJECT_LENGTH, (PUCHAR)(&KeyObjectSize), sizeof(DWORD), &res, 0))
			{
				throw PCSCCryptographicException("BCryptGetProperty���G���[��Ԃ��܂���");
			}
			PUCHAR pKeyObject = (PUCHAR)(malloc(KeyObjectSize));
			if (pKeyObject == NULL)
			{
				throw PCSCCryptographicException("KeyObject�̃������m�ۂɎ��s");
			}

			BCRYPT_KEY_HANDLE hKey;
			try
			{
				if (BCryptGenerateSymmetricKey(hAlgorithm, &hKey, pKeyObject, KeyObjectSize, (PUCHAR)key, dataLength, 0))
				{
					throw PCSCCryptographicException("BCryptGenerateSymmetricKey���G���[��Ԃ��܂���");
				}

				if (BCryptEncrypt(hKey, (PUCHAR)input, dataLength, nullptr, (PUCHAR)IV, dataLength, (PUCHAR)output, dataLength, &res, 0))
				{
					throw PCSCCryptographicException("BCryptEncrypt���G���[��Ԃ��܂���");
				}
			}
			catch (...)
			{
				BCryptDestroyKey(hKey);
				BCryptCloseAlgorithmProvider(hAlgorithm, 0);
				free(pKeyObject);
				throw; //��ɓ�����
			}

			BCryptDestroyKey(hKey);
			BCryptCloseAlgorithmProvider(hAlgorithm, 0);
			free(pKeyObject);
		}

		//8�o�C�g�Œ蒷��DES���������s��
		void desDecryptFixedLength(const uint8_t input[8], const uint8_t key[8], const uint8_t IV[8], uint8_t output[8])
		{
			const int dataLength = 8;
			DWORD KeyObjectSize;
			BCRYPT_ALG_HANDLE hAlgorithm;
			DWORD res = 0;

			if (BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_DES_ALGORITHM, MS_PRIMITIVE_PROVIDER, 0))
			{
				throw PCSCCryptographicException("BCryptOpenAlgorithmProvider���G���[��Ԃ��܂���");
			}
			if (BCryptGetProperty(hAlgorithm, BCRYPT_OBJECT_LENGTH, (PUCHAR)(&KeyObjectSize), sizeof(DWORD), &res, 0))
			{
				throw PCSCCryptographicException("BCryptGetProperty���G���[��Ԃ��܂���");
			}
			PUCHAR pKeyObject = (PUCHAR)(malloc(KeyObjectSize));
			if (pKeyObject == NULL)
			{
				throw PCSCCryptographicException("KeyObject�̃������m�ۂɎ��s");
			}

			BCRYPT_KEY_HANDLE hKey;
			try
			{
				if (BCryptGenerateSymmetricKey(hAlgorithm, &hKey, pKeyObject, KeyObjectSize, (PUCHAR)key, dataLength, 0))
				{
					throw PCSCCryptographicException("BCryptGenerateSymmetricKey���G���[��Ԃ��܂���");
				}

				if (BCryptDecrypt(hKey, (PUCHAR)input, dataLength, nullptr, (PUCHAR)IV, dataLength, (PUCHAR)output, dataLength, &res, 0))
				{
					throw PCSCCryptographicException("BCryptDecrypt���G���[��Ԃ��܂���");
				}
			}
			catch (...)
			{
				BCryptDestroyKey(hKey);
				BCryptCloseAlgorithmProvider(hAlgorithm, 0);
				free(pKeyObject);
				throw; //��ɓ�����
			}

			BCryptDestroyKey(hKey);
			BCryptCloseAlgorithmProvider(hAlgorithm, 0);
			free(pKeyObject);
		}

		//�o�C�g�I�[�_�[�����ւ���(�|�C���^�̐�𒼂ɓ���ւ���̂Œ��ӁI)
		void swapByteOrder(uint8_t inout[8])
		{
			uint8_t swap[8];
			for (int i = 0; i < 8; i++)
			{
				swap[7 - i] = inout[i];
			}
			for (int i = 0; i < 8; i++)
			{
				inout[i] = swap[i];
			}
		}

		//�f�o�b�O�p:8Byte��16�i���̕\��
		void showMAC(const uint8_t input[8])
		{
			for (int i = 0; i < 8; i++)
			{
				printf("%02X ", input[i]);
			}
			printf("\n");
		}

		//8�o�C�g��XOR
		void arryXor(const uint8_t input1[8], const uint8_t input2[8], uint8_t output[8])
		{
			for (int i = 0; i < 8; i++)
			{
				output[i] = input1[i] ^ input2[i];
			}
		}

		//�������x�N�g����������
		void zeroIV(uint8_t IV[8])
		{
			for (int i = 0; i < 8; i++)
			{
				IV[i] = 0;
			}
		}

		//2key-3DES���s���D
		void Key2des3(const uint8_t input[8], const uint8_t XorInput[8], const uint8_t key1[8], const uint8_t key2[8], uint8_t output[8])
		{
			uint8_t IV[8], tmp[8], tmp2[8];
			arryXor(input, XorInput, tmp2);

			zeroIV(IV);
			desEncryptFixedLength(tmp2, key1, IV, tmp);
			zeroIV(IV);
			desDecryptFixedLength(tmp, key2, IV, tmp2);
			zeroIV(IV);
			desEncryptFixedLength(tmp2, key1, IV, output);
		}

		//2key-3DES���s���D�A��Felica Lite�p�Ƀo�C�g�I�[�_�[����ւ������ƁC2�A���������Ă���
		void DualKey2des3(const uint8_t input1[8], const uint8_t input2[8], const uint8_t XorInput[8], const uint8_t key1[8], const uint8_t key2[8], uint8_t output1[8], uint8_t output2[8])
		{
			uint8_t Tinput1[8], Tinput2[8], TXorInput[8], Tkey1[8], Tkey2[8];

			//���f�[�^��j�󂵂Ȃ�����
			for (int i = 0; i < 8; i++)
			{
				Tinput1[i] = input1[i];
				Tinput2[i] = input2[i];
				TXorInput[i] = XorInput[i];
				Tkey1[i] = key1[i];
				Tkey2[i] = key2[i];
			}

			swapByteOrder(Tinput1);
			swapByteOrder(Tinput2);
			swapByteOrder(TXorInput);
			swapByteOrder(Tkey1);
			swapByteOrder(Tkey2);

			Key2des3(Tinput1, TXorInput, Tkey1, Tkey2, output1);
			Key2des3(Tinput2, output1, Tkey1, Tkey2, output2);

			swapByteOrder(output1);
			swapByteOrder(output2);
		}

		//Felica Lite�Z�b�V�����L�[�𐶐�����
		void makeSessionKey(uint8_t RC1[8], uint8_t RC2[8], uint8_t CK1[8], uint8_t CK2[8], uint8_t SK1[8], uint8_t SK2[8])
		{
			uint8_t ZERO[8] = { 0 };

			DualKey2des3(RC1, RC2, ZERO, CK1, CK2, SK1, SK2);
			debugInfo2(__func__, " �Z�b�V�����L�[�𐶐����܂���\n");
			if (debugFlag)
			{
				debugInfo2(__func__, " SK1 ");
				showMAC(SK1);
				debugInfo2(__func__, " SK2 ");
				showMAC(SK2);
			}
		}

		//Felica Lite��MAC�𐶐�����D(Felica Lite-S��MAC_A�ł͂Ȃ��IFelica Lite���[�U�[�Y�}�j���A����ǂނ��ƁI)
		void makeMAC(uint8_t BLOCK[16], uint8_t RC1[8], uint8_t SK1[8], uint8_t SK2[8], uint8_t MAC[8])
		{
			uint8_t BLOCK_HIGH[8], BLOCK_LOW[8];
			uint8_t UNUSED[8]; //���ԕϐ��D�̂Ă�

			for (int i = 0; i < 8; i++)
			{
				BLOCK_HIGH[i] = BLOCK[8 + i];
				BLOCK_LOW[i] = BLOCK[0 + i];
			}
			DualKey2des3(BLOCK_LOW, BLOCK_HIGH, RC1, SK1, SK2, UNUSED, MAC);
			debugInfo2(__func__, " MAC�𐶐����܂���\n");
			if (debugFlag)
			{
				debugInfo2(__func__, " MAC ");
				showMAC(MAC);
			}
		}

		//Felica Lite-S�̓ǂݏo����MAC_A�𐶐�����D(Felica Lite��MAC�ł͂Ȃ��IFelica Lite-S���[�U�[�Y�}�j���A����ǂނ��ƁI)
		void makeReadMAC_A(uint16_t BlockAdr, uint8_t BLOCK[16], uint8_t RC1[8], uint8_t SK1[8], uint8_t SK2[8], uint8_t MAC[8])
		{
			uint8_t BLOCK_HIGH[8], BLOCK_LOW[8];
			uint8_t tmp1[8], tmp2[8]; //���ԕϐ�
			uint8_t Tinput1[8], Tinput2[8], TXorInput[8], Tkey1[8], Tkey2[8], TBlockInfo[8];
			uint8_t ZERO[8] = { 0 };
			for (int i = 0; i < 8; i++)
			{
				BLOCK_HIGH[i] = BLOCK[8 + i];
				BLOCK_LOW[i] = BLOCK[0 + i];
			}
			uint8_t BlockInfo[8] = { (uint8_t)(BlockAdr & 0xFF),(uint8_t)((BlockAdr >> 8) & 0xFF),(uint8_t)(ADDRESS_MAC_A & 0xFF),(uint8_t)((ADDRESS_MAC_A >> 8) & 0xFF),0xFF,0xFF,0xFF,0xFF };

			//���f�[�^��j�󂵂Ȃ�����
			for (int i = 0; i < 8; i++)
			{
				TBlockInfo[i] = BlockInfo[i];
				Tinput1[i] = BLOCK_LOW[i];
				Tinput2[i] = BLOCK_HIGH[i];
				TXorInput[i] = RC1[i];
				Tkey1[i] = SK1[i];
				Tkey2[i] = SK2[i];
			}

			swapByteOrder(TBlockInfo);
			swapByteOrder(Tinput1);
			swapByteOrder(Tinput2);
			swapByteOrder(TXorInput);
			swapByteOrder(Tkey1);
			swapByteOrder(Tkey2);

			Key2des3(TBlockInfo, TXorInput, Tkey1, Tkey2, tmp1);
			Key2des3(Tinput1, tmp1, Tkey1, Tkey2, tmp2);
			Key2des3(Tinput2, tmp2, Tkey1, Tkey2, MAC);

			swapByteOrder(MAC);

			debugInfo2(__func__, " MAC_A�𐶐����܂���\n");
			if (debugFlag)
			{
				debugInfo2(__func__, " MAC_A ");
				showMAC(MAC);
			}
		}

		//Felica Lite-S�̏������ݎ�MAC_A�𐶐�����D(Felica Lite��MAC�ł͂Ȃ��IFelica Lite-S���[�U�[�Y�}�j���A����ǂނ��ƁI)
		void makeWriteMAC_A(uint16_t BlockAdr, uint8_t WCNT[8], uint8_t BLOCK[16], uint8_t RC1[8], uint8_t SK1[8], uint8_t SK2[8], uint8_t MAC[8])
		{
			uint8_t BLOCK_HIGH[8], BLOCK_LOW[8];
			uint8_t tmp1[8], tmp2[8]; //���ԕϐ�
			uint8_t Tinput1[8], Tinput2[8], TXorInput[8], Tkey1[8], Tkey2[8], TBlockInfo[8];
			uint8_t ZERO[8] = { 0 };

			if ((WCNT[0] == 0xFF && WCNT[1] == 0xFF && WCNT[2] == 0xFF) || (WCNT[0] == 0x00 && WCNT[1] == 0xFE && WCNT[2] == 0xFF))
			{
				throw FelicaFatalException("�������݉񐔂����E�̂���MAC_A�������݂ł��܂���", FelicaFatalException::WCNT_CLIP);
			}

			for (int i = 0; i < 8; i++)
			{
				BLOCK_HIGH[i] = BLOCK[8 + i];
				BLOCK_LOW[i] = BLOCK[0 + i];
			}
			uint8_t BlockInfo[8] = { WCNT[0],WCNT[1],WCNT[2],WCNT[3],(uint8_t)(BlockAdr & 0xFF),(uint8_t)((BlockAdr >> 8) & 0xFF),(uint8_t)(ADDRESS_MAC_A & 0xFF),(uint8_t)((ADDRESS_MAC_A >> 8) & 0xFF) };
			//���f�[�^��j�󂵂Ȃ�����
			for (int i = 0; i < 8; i++)
			{
				TBlockInfo[i] = BlockInfo[i];
				Tinput1[i] = BLOCK_LOW[i];
				Tinput2[i] = BLOCK_HIGH[i];
				TXorInput[i] = RC1[i];
				Tkey1[i] = SK1[i];
				Tkey2[i] = SK2[i];
			}

			swapByteOrder(TBlockInfo);
			swapByteOrder(Tinput1);
			swapByteOrder(Tinput2);
			swapByteOrder(TXorInput);
			swapByteOrder(Tkey1);
			swapByteOrder(Tkey2);

			//Key������ւ���Ă��邱�Ƃɒ���
			Key2des3(TBlockInfo, TXorInput, Tkey2, Tkey1, tmp1);
			Key2des3(Tinput1, tmp1, Tkey2, Tkey1, tmp2);
			Key2des3(Tinput2, tmp2, Tkey2, Tkey1, MAC);

			swapByteOrder(MAC);

			debugInfo2(__func__, " MAC_A�𐶐����܂���\n");
			if (debugFlag)
			{
				debugInfo2(__func__, " MAC_A ");
				showMAC(MAC);
			}
		}

		//MAC�𐶐����Ĕ�r����
		bool compareMAC(uint8_t BLOCK[16], uint8_t MAC[16], uint8_t RC1[8], uint8_t RC2[8], uint8_t CK1[8], uint8_t CK2[8])
		{
			uint8_t SK1[8], SK2[8], CALC_MAC[16] = { 0 };
			makeSessionKey(RC1, RC2, CK1, CK2, SK1, SK2);
			makeMAC(BLOCK, RC1, SK1, SK2, CALC_MAC);

			for (int i = 0; i < 8; i++)
			{
				if (MAC[i] != CALC_MAC[i])
				{
					debugInfo2(__func__, " MAC�s��v\n");
					return false;
				}
			}
			debugInfo2(__func__, " MAC��v\n");
			return true;
		}

		//MAC_A�𐶐����Ĕ�r����
		bool compareMAC_A(uint16_t BlockAdr, uint8_t BLOCK[16], uint8_t MAC[16], uint8_t RC1[8], uint8_t RC2[8], uint8_t CK1[8], uint8_t CK2[8])
		{
			uint8_t SK1[8], SK2[8], CALC_MAC[16] = { 0 };
			makeSessionKey(RC1, RC2, CK1, CK2, SK1, SK2);
			makeReadMAC_A(BlockAdr, BLOCK, RC1, SK1, SK2, CALC_MAC);

			for (int i = 0; i < 8; i++)
			{
				if (MAC[i] != CALC_MAC[i])
				{
					debugInfo2(__func__, " MAC�s��v\n");
					return false;
				}
			}
			debugInfo2(__func__, " MAC��v\n");
			return true;
		}

		//RandomChallenge���J�[�h�ɏ�������
		void writeRandomChallenge(uint8_t RC1[8], uint8_t RC2[8])
		{
			uint8_t RC[16];
			for (int i = 0; i < 8; i++)
			{
				RC[i] = RC1[i];
				RC[i + 8] = RC2[i];
			}

			updateBinary(ADDRESS_RC, RC);
			debugInfo2(__func__, " RC���������݂܂���\n");
		}
		//CardKey���J�[�h�ɏ�������(CK���������݌�́CRC����������ŃZ�b�V���������Đ��������邱��)
		void writeCardKey(uint8_t CK1[8], uint8_t CK2[8])
		{
			uint8_t CK[16];
			for (int i = 0; i < 8; i++)
			{
				CK[i] = CK1[i];
				CK[i + 8] = CK2[i];
			}

			updateBinary(ADDRESS_CK, CK);
			debugInfo2(__func__, " CK���������݂܂���\n");
		}

		//�J�[�h�����g���CMAC��ǂݍ����IDm�̌��؂��s���̂������ōs���D�J�[�h���Ǝ����̎����Ă��錮�����������̌��؂ɂȂ�D
		bool cardIdCheckMAC(uint8_t CK1[8], uint8_t CK2[8])
		{
			uint8_t dat[16], mac[16];
			uint8_t RC1[8], RC2[8];

			makeRandomChallenge(RC1, RC2);
			writeRandomChallenge(RC1, RC2);
			readBinaryWithMAC(ADDRESS_ID, dat, mac);
			if (!compareMAC(dat, mac, RC1, RC2, CK1, CK2))
			{
				debugInfo2(__func__, " �J�[�h���ƍ��Ɏ��s\n");
				return false;
			}
			debugInfo2(__func__, " �J�[�h���ƍ��ɐ���\n");
			return true;
		}

		//�J�[�h�����g���CMAC_A��ǂݍ����IDm�̌��؂��s���̂������ōs���DFelica Lite-S��p
		bool cardIdCheckMAC_A(uint8_t CK1[8], uint8_t CK2[8])
		{
			uint8_t dat[16], mac[16];
			uint8_t RC1[8], RC2[8];

			makeRandomChallenge(RC1, RC2);
			writeRandomChallenge(RC1, RC2);
			readBinaryWithMAC_A(ADDRESS_ID, dat, mac);
			if (!compareMAC_A(ADDRESS_ID, dat, mac, RC1, RC2, CK1, CK2))
			{
				debugInfo2(__func__, " �J�[�h���ƍ��Ɏ��s\n");
				return false;
			}
			debugInfo2(__func__, " �J�[�h���ƍ��ɐ���\n");
			return true;
		}

		//RandomChallenge�𗐐����琶������
		void makeRandomChallenge(uint8_t RC1[8], uint8_t RC2[8])
		{
			for (int i = 0; i < 8; i++)
			{
				RC1[i] = CryptRand();
				RC2[i] = CryptRand();
			}

			debugInfo2(__func__, " RC�𐶐����܂���\n");
			if (debugFlag)
			{
				debugInfo2(__func__, " RC1 ");
				showMAC(RC1);
				debugInfo2(__func__, " RC2 ");
				showMAC(RC2);
			}
		}

		//STATE��L���ɂ���D���Ƃɖ߂��Ȃ��D
		void enableSTATE()
		{
			if (!isFelicaLite())
			{
				throw FelicaFatalException("Felica Lite�ł͂Ȃ��J�[�h�����������邱�Ƃ͂ł��܂���", FelicaFatalException::CARD_IS_NOT_FELICA_LITE);
			}

			uint8_t mc[16];
			readBinary(ADDRESS_MC, mc);
			mc[12] = 0x01;
			updateBinary(ADDRESS_MC, mc);
			debugInfo2(__func__, " STATE��MAC�t���������݂�L���ɂ��܂���\n");
		}

		//STATE��EXT_AUTH�𗧂Ă���܂����肷��(STATE��L���ɂ��Ȃ��Ǝg���܂���)
		void setSTATE_EXT_AUTH(bool auth, uint8_t CK1[8], uint8_t CK2[8])
		{
			uint8_t state[16] = { 0 };
			readBinary(ADDRESS_STATE, state);

			if (auth)
			{
				debugInfo2(__func__, " �O���F�؂��J�n���܂�\n");
				state[0] = 0x01;
			}
			else {
				debugInfo2(__func__, " �O���F�؂��I�����܂�\n");
				state[0] = 0x00;
			}
			updateBinaryWithMAC_A_Auto(ADDRESS_STATE, state, CK1, CK2);
			debugInfo2(__func__, " �O���F�؏����ɐ������܂���\n");
		}

		//MAC�t���������݂��C�Z�b�V�����L�[�̐������玩���ōs��
		void updateBinaryWithMAC_A_Auto(uint16_t adr, uint8_t dat[16], uint8_t CK1[8], uint8_t CK2[8])
		{
			uint8_t mac[16], wcnt[16];
			uint8_t RC1[8], RC2[8], SK1[8], SK2[8];

			debugInfo2(__func__, " ����MAC_A�t���������݊J�n\n");

			makeRandomChallenge(RC1, RC2);
			debugInfo2(__func__, " RC��������\n");

			writeRandomChallenge(RC1, RC2);
			debugInfo2(__func__, " RC�������݊���\n");

			makeSessionKey(RC1, RC2, CK1, CK2, SK1, SK2);
			debugInfo2(__func__, " �Z�b�V�����L�[��������\n");

			readBinary(ADDRESS_WCNT, wcnt);
			debugInfo2(__func__, " WCNT�ǂݎ�芮��\n");
			debugInfo2(__func__, " WCNT: ");
			showMAC(wcnt);

			makeWriteMAC_A(adr, wcnt, dat, RC1, SK1, SK2, mac);
			debugInfo2(__func__, " ��������MAC_A��������\n");

			mac[8] = wcnt[0];
			mac[9] = wcnt[1];
			mac[10] = wcnt[2];
			mac[11] = wcnt[3];
			updateBinaryWithMAC_A(adr, dat, mac);
			debugInfo2(__func__, " MAC_A�t���������݊���\n");
		}

		//�Í��_�I�[�������𐶐�����
		uint8_t CryptRand()
		{
			HCRYPTPROV hProv;
			BYTE buf[1];

			if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0))
			{
				throw PCSCCryptographicException("CryptAcquireContext���G���[��Ԃ��܂���");
			}

			if (!CryptGenRandom(hProv, 1, buf))
			{
				throw PCSCCryptographicException("CryptGenRandom���G���[��Ԃ��܂���");
			}

			CryptReleaseContext(hProv, NULL);

			return buf[0];
		}

	};
}