#pragma once
// ������, ����������� ������ ���������� WinSock
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>	// winsock2.h ������ ���� ��������� ������ windows.h
#include <windows.h>
#include <process.h>	// ��� _beginthreadex 
#include <vector>
#include <fstream>
#include <string>
#pragma comment(lib, "ws2_32.lib") 

// ������ ������ ��������
#define BUFFER_SIZE 1024
// ����, ������� ������� ������
#define MY_PORT 666

// ���������� ������ � �����
#define ROOM_COUNT 10

// ���������, ����� ������� ����� ������������ ��������� � ServeClient
typedef struct
{
	SOCKET* client_socket; // ����� �������
	int client_id;			// ID �������
} serve_client_params_t;

// �������� �������, ������������� �������������� �������������
unsigned __stdcall ServeClient(void* params);

// �������� �������, ����������� �����������
unsigned __stdcall AcceptConnections(void* params);

// ������� ���������� ��������� �� ������
void showErrorDialog(System::String^ msg);
// ������� ���������� ��������� �� ������ � ��������� ������� �� ����������
void showErrorDialogAndSafelyExit(System::String^ msg);
// �������, ������� � ���
void toLog(System::String^ msg);

// ���������� �������� ������������� 
int nclients = 0;

int current_time = 0;

SOCKET mysocket;
bool closed = false; // ��� �� ������ ����������

HANDLE semaphore; // ������� ��� ������� � ������
HANDLE mutex; // ������� ��� ������� � ��������

std::vector<bool> occupied; // ������ �� ������� ��� ���
namespace server {
	using namespace System;
	using namespace System::IO;
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

			// ��� 1 - ������������� ���������� �������
			WSADATA ws;
			if (WSAStartup(0x0202, &ws))
			{
				// ������
				showErrorDialog("Error WSAStartup: " + WSAGetLastError());
				// unsafe exit
				closed = true;
				if (components) delete components;
				System::Environment::Exit(-1);
			}

			// ��� 2 - �������� ������
			// >>>>��. ���������� �����.>>>> SOCKET mysocket;
			// AF_INET - ����� ���������
			// SOCK_STREAM - ��������� ����� (� ���������� ����������)
			// 0 - �� ��������� ���������� TCP ��������
			if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				// ������
				showErrorDialog("Error creating socket: " + WSAGetLastError());
				// �������������� ���������� Winsock
				WSACleanup();

				// unsafe exit
				closed = true;
				if (components) delete components;
				System::Environment::Exit(-1);
			}

			// ��� 3 - ���������� ������ � ��������� �������
			sockaddr_in local_addr;
			local_addr.sin_family = AF_INET;
			local_addr.sin_port = htons(MY_PORT);	// �� �������� � ������� �������
			local_addr.sin_addr.s_addr = 0;			// ������ ��������� ����������� �� ��� IP-������

			// �������� bind ��� ����������
			if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
			{
				// ������
				showErrorDialogAndSafelyExit("Error binding: " + WSAGetLastError());
			}

			// ��� 4 - �������� �����������
			// ������ ������� � 0x100
			if (listen(mysocket, 0x100))
			{
				// ������
				showErrorDialogAndSafelyExit("Error listening: " + WSAGetLastError());
			}

		}

	protected:
		/// <summary>
		/// ���������� ��� ������������ �������.
		/// </summary>
		~MyForm()
		{
			closed = true;

			closesocket(mysocket);
			WSACleanup();

			if (components)
			{
				delete components;
			}
		}
	public: static System::Windows::Forms::DataGridView^ dataGridView1;
	protected:
	public: static System::Windows::Forms::RichTextBox^ textBox_log;

	protected:




	protected:

	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;

	private: System::Windows::Forms::DataGridViewTextBoxColumn^ RoomColumn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ IdColumn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ NameColumn;

	private:
		/// <summary>
		/// ������������ ���������� ������������.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ��������� ����� ��� ��������� ������������ � �� ��������� 
		/// ���������� ����� ������ � ������� ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->RoomColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->IdColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->NameColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox_log = (gcnew System::Windows::Forms::RichTextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {
				this->RoomColumn,
					this->IdColumn, this->NameColumn
			});
			this->dataGridView1->Location = System::Drawing::Point(17, 21);
			this->dataGridView1->Margin = System::Windows::Forms::Padding(2);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 24;
			this->dataGridView1->Size = System::Drawing::Size(322, 275);
			this->dataGridView1->TabIndex = 0;
			this->dataGridView1->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &MyForm::dataGridView1_CellContentClick);
			// 
			// RoomColumn
			// 
			this->RoomColumn->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->RoomColumn->HeaderText = L"�������";
			this->RoomColumn->Name = L"RoomColumn";
			this->RoomColumn->ReadOnly = true;
			this->RoomColumn->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->RoomColumn->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->RoomColumn->Width = 57;
			// 
			// IdColumn
			// 
			this->IdColumn->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
			this->IdColumn->HeaderText = L"ID";
			this->IdColumn->Name = L"IdColumn";
			this->IdColumn->ReadOnly = true;
			this->IdColumn->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->IdColumn->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			this->IdColumn->Width = 24;
			// 
			// NameColumn
			// 
			this->NameColumn->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->NameColumn->HeaderText = L"���";
			this->NameColumn->Name = L"NameColumn";
			this->NameColumn->ReadOnly = true;
			this->NameColumn->Resizable = System::Windows::Forms::DataGridViewTriState::False;
			this->NameColumn->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Font = (gcnew System::Drawing::Font(L"Calibri", 12));
			this->button1->Location = System::Drawing::Point(148, 307);
			this->button1->Margin = System::Windows::Forms::Padding(2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(191, 29);
			this->button1->TabIndex = 1;
			this->button1->Text = L"��������� ����";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Calibri", 12));
			this->label1->Location = System::Drawing::Point(14, 312);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(56, 19);
			this->label1->TabIndex = 2;
			this->label1->Text = L"���� 0";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Calibri", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(14, 345);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(96, 19);
			this->label2->TabIndex = 4;
			this->label2->Text = L"��� �������";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
			// 
			// textBox_log
			// 
			this->textBox_log->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_log->Location = System::Drawing::Point(17, 367);
			this->textBox_log->Margin = System::Windows::Forms::Padding(2);
			this->textBox_log->Name = L"textBox_log";
			this->textBox_log->ReadOnly = true;
			this->textBox_log->Size = System::Drawing::Size(323, 74);
			this->textBox_log->TabIndex = 5;
			this->textBox_log->Text = L"";
			this->textBox_log->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox_log_TextChanged);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(363, 456);
			this->Controls->Add(this->textBox_log);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->dataGridView1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->MinimumSize = System::Drawing::Size(379, 495);
			this->Name = L"MyForm";
			this->Text = L"����� (������)";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MyForm::MyForm_FormClosed);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {

		dataGridView1->Rows->Add(ROOM_COUNT);
		for (size_t i = 0; i < ROOM_COUNT; i++)
		{
			dataGridView1["RoomColumn", i]->Value = i;
		}
		dataGridView1->ClearSelection();

		// vector init
		occupied = std::vector<bool>(ROOM_COUNT);

		textBox_log->Text += "...��� �����������...\n";

		semaphore = CreateSemaphore(NULL, ROOM_COUNT, ROOM_COUNT, NULL);
		mutex = CreateMutex(NULL, FALSE, NULL);

		_beginthreadex(NULL, 0, AcceptConnections, NULL, 0, NULL); // ����� ����������

	}
		   //������� �� ���� - ������� � ���������� ���
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		current_time++;
		label1->Text = "���� " + current_time;
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void textBox_log_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void dataGridView1_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
}
private: System::Void MyForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e) {
	System::String^ str = textBox_log->Text;
	System::IO::StreamWriter^ write = gcnew System::IO::StreamWriter("log.txt", true);
	(*write).Write(str);
	(*write).Close();

}
};

}

// ������ ����������� ������� � ��������� ������, ������ ��� �� ������ �������
unsigned __stdcall AcceptConnections(void* params) {
	// 5.  ��������� ��������� �� �������
	SOCKET client_socket;		// ����� ��� �������
	sockaddr_in client_addr;    // ����� ������� (����������� ��������)

	// ������� accept ���������� �������� ������ ���������
	int client_addr_size = sizeof(client_addr);

	// ID ���������� �������
	int idclients = 0;

	// ���� ���������� �������� �� ����������� �� �������
	while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, &client_addr_size)))
	{
		if (closed) return -1;

		nclients++; // ����������� ������� �������������� ��������

		// �������� �������� ��� �����
		HOSTENT* hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);

		// �������� ��������� ��� ������� ServeClient
		serve_client_params_t params;
		params.client_socket = &client_socket;
		params.client_id = idclients++;

		// ����� ������ ������ ��� ����������� �������
		_beginthreadex(NULL, 0, ServeClient, &params, 0, NULL);

	}

	return 0;
}

// ��� ������� ��������� � ��������� ������ � ���������� ���������� ��������������� ������� ���������� �� ���������
unsigned __stdcall ServeClient(void* params)
{
	// ������� ���������
	serve_client_params_t* ptr = (serve_client_params_t*)params;
	SOCKET my_sock = *(ptr->client_socket); // ����� �������
	int id = ptr->client_id;				// ID �������

	// id_char ��������� �� id ��� �� ������ �� 4 char
	char* id_char = (char*)&id;
	// �������� ��� �������
	send(my_sock, id_char, 4, 0);

	// ���������� �����
	char buff[BUFFER_SIZE];

	// �������� ��� �������
	if (recv(my_sock, buff, sizeof(buff), 0) == SOCKET_ERROR) {

		showErrorDialogAndSafelyExit("Recv error: " + WSAGetLastError());
	}

	// ������������ ���������� �������
	int duration;
	char dur_char[4];
	if (recv(my_sock, dur_char, sizeof(dur_char), 0) == SOCKET_ERROR) {
		// recv ������� ������
		showErrorDialogAndSafelyExit("Recv error: " + WSAGetLastError());
	}
	duration = *((int*)dur_char);

	// ���� ��������� ������
	WaitForSingleObject(semaphore, INFINITE);
	// ������� ������ ������

	int startTime = current_time;

	// check in procedure
	WaitForSingleObject(mutex, INFINITE);
	int i; // given room
	for (i = 0; i < ROOM_COUNT; i++)
	{
		if (!occupied[i]) {
			break;
		}
	}
	occupied[i] = true;

	server::MyForm::dataGridView1["NameColumn", i]->Value = gcnew System::String(buff);

	send(my_sock, (char*)&i, 4, 0);

	server::MyForm::dataGridView1["IdColumn", i]->Value = id;

	toLog("���� " + current_time + ": ������ " +
		(gcnew System::String(buff)) + " (id = " + id + ") ���������� � ������� " + i +
		" �� " + duration + " ����(����)\n");
	ReleaseMutex(mutex);
	// check in ends

	// sleep on soft bed
	while (current_time < (startTime + duration)) {
		Sleep(5);
	}

	// check out procedure
	WaitForSingleObject(mutex, INFINITE);
	occupied[i] = false;

	server::MyForm::dataGridView1["NameColumn", i]->Value = "";
	server::MyForm::dataGridView1["IdColumn", i]->Value = "";

	const char* goaway = "leave";
	send(my_sock, goaway, sizeof(goaway), 0);

	toLog("���� " + current_time + ": ������ " +
		(gcnew System::String(buff)) + " (id = " + id + ") ��������� �� ������� " + i + "\n");
	ReleaseMutex(mutex);
	// check out ends

	ReleaseSemaphore(semaphore, 1, NULL);
	// leave the building

	nclients--; // ��������� ������� �������� ��������

	// ��������� �����
	closesocket(my_sock);

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
	server::MyForm::textBox_log->Text = msg + server::MyForm::textBox_log->Text;
}
void toLog(System::String^ msg) {
	server::MyForm::textBox_log->Invoke(gcnew AddMessageDelegate(LogAdd), { msg });
}
