#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace AziMap {

	/// <summary>
	/// ChSrc �̊T�v
	///
	/// �x��: ���̃N���X�̖��O��ύX����ꍇ�A���̃N���X���ˑ����邷�ׂĂ� .resx �t�@�C���Ɋ֘A�t����ꂽ
	///          �}�l�[�W ���\�[�X �R���p�C�� �c�[���ɑ΂��� 'Resource File Name' �v���p�e�B��
	///          �ύX����K�v������܂��B���̕ύX���s��Ȃ��ƁA
	///          �f�U�C�i�ƁA���̃t�H�[���Ɋ֘A�t����ꂽ���[�J���C�Y�ς݃��\�[�X�Ƃ��A
	///          ���������݂ɗ��p�ł��Ȃ��Ȃ�܂��B
	/// </summary>
	public ref class ChSrc : public System::Windows::Forms::Form
	{
	public:
		ChSrc(String^ src)
		{
			InitializeComponent();
			//
			//TODO: �����ɃR���X�g���N�^ �R�[�h��ǉ����܂�
			//
			this->srcimg = src;
		}

	protected:
		/// <summary>
		/// �g�p���̃��\�[�X�����ׂăN���[���A�b�v���܂��B
		/// </summary>
		~ChSrc()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		String^ srcimg;
	private: System::Windows::Forms::Button^  button1;
	protected: 
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	private:
		/// <summary>
		/// �K�v�ȃf�U�C�i�ϐ��ł��B
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �f�U�C�i �T�|�[�g�ɕK�v�ȃ��\�b�h�ł��B���̃��\�b�h�̓��e��
		/// �R�[�h �G�f�B�^�ŕύX���Ȃ��ł��������B
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(52, 24);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 28);
			this->button1->TabIndex = 0;
			this->button1->Text = L"Original";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ChSrc::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(52, 67);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 28);
			this->button2->TabIndex = 1;
			this->button2->Text = L"From file";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ChSrc::button2_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(115, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Choose source image";
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// ChSrc
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(183, 121);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"ChSrc";
			this->Text = L"ChSrc";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:
		String^ getSrcImg(){
			return this->srcimg;
		}
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->srcimg = "";
				 this->Close();
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->openFileDialog1->InitialDirectory = System::Environment::CurrentDirectory;
				 this->openFileDialog1->ShowDialog();
				 this->srcimg = this->openFileDialog1->FileName;
				 this->Close();
			 }
	};
}
