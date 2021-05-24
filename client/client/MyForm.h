#pragma once

// ������, ����������� ������ ���������� WinSock
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <winsock2.h> // winsock2.h ������ ���� ��������� ������ windows.h
#include <windows.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib") 

// ������ ������ ��������
#define BUFFER_SIZE 1024

// ���� ��� �����������
#define PORT 666
// ����� ��� �����������
#define SERVERADDR "127.0.0.1"

// ������� ���������� ��������� �� ������
void showErrorDialog(System::String^ msg);
// ������� ���������� ��������� �� ������ � ��������� ������� �� ����������
void showErrorDialogAndSafelyExit(System::String^ msg);
// �������, ������� � ���
void toLog(System::String^ msg);

unsigned __stdcall Timeline(void* params);

SOCKET my_sock;

namespace client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// ������ ��� MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: �������� ��� ������������
			//

			// 1. ������������� ���������� Winsock
			WSADATA ws;
			if (WSAStartup(0x202, &ws))
			{
				showErrorDialog("WSAStartup error: " + WSAGetLastError());

				// unsafe exit
				if (components) delete components;
				System::Environment::Exit(-1);
			}

			// 2. �������� ������
			/*SOCKET*/
			my_sock = socket(AF_INET, SOCK_STREAM, 0);
			if (my_sock < 0)
			{

				showErrorDialog("Error creating socket: " + WSAGetLastError());
				WSACleanup();


				// unsafe exit
				if (components) delete components;
				System::Environment::Exit(-1);
			}

			// 3.��������� ����������

			// ���������� ��������� sockaddr_in
			// �������� ������ � ����� �������
			sockaddr_in dest_addr;
			dest_addr.sin_family = AF_INET;
			dest_addr.sin_port = htons(PORT);

			HOSTENT* hst;

			// �������������� IP ������ �� ����������� � ������� ������
			if (inet_addr(SERVERADDR) != INADDR_NONE)
				dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
			else
				// ������� �������� IP ����� �� ��������� ����� �������
				if (hst = gethostbyname(SERVERADDR))
					// hst->h_addr_list �������� �� ������ �������, � ������ ���������� �� ������
					((unsigned long*)&dest_addr.sin_addr)[0] =
					((unsigned long**)hst->h_addr_list)[0][0];
				else
				{
					showErrorDialogAndSafelyExit("Invalid address: " + SERVERADDR);
				}

			// ����� ������� �������  �������� ���������� ���������� 
			if (connect(my_sock, (sockaddr*)&dest_addr, sizeof(dest_addr)))
			{
				showErrorDialogAndSafelyExit("Error connecting: " + WSAGetLastError());
			}

		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~MyForm()
		{
			closesocket(my_sock);
			WSACleanup();

			if (components)
			{
				delete components;
			}
		}
	public: static System::Windows::Forms::TextBox^ textBox1;
	public: static System::Windows::Forms::RichTextBox^ textBox_log;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ button1;

	private:
		/// <summary>
		/// ������������ ���������� ������������
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������
		/// ���������� ����� ������ � ������� ��������� ����
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->textBox_log = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->textBox1->Location = System::Drawing::Point(16, 36);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(160, 26);
			this->textBox1->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(12, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(47, 20);
			this->label1->TabIndex = 1;
			this->label1->Text = L"���:";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(16, 87);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(125, 24);
			this->button1->TabIndex = 4;
			this->button1->Text = L"�������!";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// textBox_log
			// 
			this->textBox_log->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_log->Location = System::Drawing::Point(196, 12);
			this->textBox_log->Name = L"textBox_log";
			this->textBox_log->Size = System::Drawing::Size(274, 179);
			this->textBox_log->TabIndex = 5;
			this->textBox_log->Text = L"";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(482, 203);
			this->Controls->Add(this->textBox_log);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->MinimumSize = System::Drawing::Size(500, 250);
			this->Name = L"MyForm";
			this->Text = L"������";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		textBox_log->Text += "����� �� ������ �����\n";

		// 4. ������ � �������� ���������

		// id_char ����� ��������� id ��� ������ �� 4 char
		char id_char[4];
		// �������� id
		if (recv(my_sock, id_char, sizeof(id_char), 0) == SOCKET_ERROR) {
			showErrorDialogAndSafelyExit("Recv error: " + WSAGetLastError());
		}
		// ��������� id
		int id = *((int*)id_char);

		textBox_log->Text += ("ID �������: " + id + "\n\n");
	}

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {

		textBox1->Enabled = false;
		button1->Enabled = false;

		_beginthreadex(NULL, 0, Timeline, NULL, 0, NULL);
	}

	};
}

unsigned __stdcall Timeline(void* params) {

	// ���������� �����
	char buff[BUFFER_SIZE];

	array<wchar_t>^ temp = client::MyForm::textBox1->Text->ToCharArray();
	int idx;
	for (idx = 0; idx < temp->Length; idx++)
	{
		buff[idx] = (char)temp[idx];
	}
	buff[idx] = 0;
	// �������� ��� ������� �������
	send(my_sock, buff, sizeof(buff), 0);

	// ����� ��������������� ������������
	int dur = 1;
	char* dur_char = (char*)&dur;
	send(my_sock, dur_char, 4, 0);

	toLog("���� �������...\n");

	// i - ������ �������
	char i_char[4];
	// ���� i, ����� ��� ��������, ����������
	if (recv(my_sock, i_char, sizeof(i_char), 0) == SOCKET_ERROR) {
		// recv ������� ������
		showErrorDialogAndSafelyExit("Recv error: " + WSAGetLastError());
	}
	// ��������� i
	int i = *((int*)i_char);

	toLog("���������� � ������� " + i + "\n");

	// ���� ��������� � ���������, ����� ��� ��������, ��������
	if (recv(my_sock, buff, sizeof(buff), 0) == SOCKET_ERROR) {
		// recv ������� ������
		showErrorDialogAndSafelyExit("Recv error: " + WSAGetLastError());
	}

	toLog("���������� �� ������� " + i + "\n");
	toLog("����! �������� ��� ����.\n");

	return 0;
}

void showErrorDialog(System::String^ msg) {
	System::Windows::Forms::MessageBox::Show(msg + "\nPress OK to exit", "Error");
}

void showErrorDialogAndSafelyExit(System::String^ msg) {
	showErrorDialog(msg);
	System::Windows::Forms::Application::Exit();
}

delegate void AddMessageDelegate(System::String^ msg);

void LogAdd(System::String^ msg) {
	client::MyForm::textBox_log->Text += msg;
}

void toLog(System::String^ msg) {
	client::MyForm::textBox_log->Invoke(gcnew AddMessageDelegate(LogAdd), { msg });
}