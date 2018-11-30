#pragma once

#define FST_N (0)
#define FST_E (0)

/*
#define FST_N (35.0)
#define FST_E (137.0)
*/

/*
#define FST_N (-63.0)
#define FST_E (-58.0)
*/

/*
#define FST_N (8.0)
#define FST_E (77.5)
*/

namespace AziMap {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace System::Runtime::InteropServices;
	using namespace System::Reflection;
	using namespace System::Threading;

	/// <summary>
	/// Form1 の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクタ コードを追加します
			//

			//ソース画像のロード
			srcimg = "";
			Assembly^ asmbly = Assembly::GetExecutingAssembly();
			updtsrcimg(gcnew Bitmap(asmbly->GetManifestResourceStream("source.jpg")));

			//表示用画像の用意
			dbmp = gcnew Bitmap(this->pictureBox1->Width,this->pictureBox1->Height);
			dw = dbmp->Width;
			dh = dbmp->Height;
			ddt = gcnew array<Byte>((int)(dw*dh*3));

			lscth = gcnew array<double,2>((int)dw,(int)dh);
			lssrad = gcnew array<double,2>((int)dw,(int)dh);
			lscrad = gcnew array<double,2>((int)dw,(int)dh);

			this->lstype = 0;
			this->equidistantToolStripMenuItem->Checked = true;
			this->magn = 1.0;
			updtls();
			this->lsavlbl = true;

			//メインの処理
			plt = (90.0 - FST_N)/180*PI;
			plg = FST_E/180*PI;

			mainfunc();
			updtimg();

			this->enthd = true;
			this->bsythd = false;
			this->ordthd = false;
			this->thd = gcnew Thread(gcnew ThreadStart(this,&Form1::thdfunc));
			this->thd->Start();
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Form1()
		{
			this->enthd = false;
			this->thd->Join();

			if (components)
			{
				delete components;
			}
		}
	private:
		String^ srcimg;

		Bitmap^ sbmp;
		BitmapData^ sbmpdt;
		array<Byte>^ sdt;
		double sw,sh;
		int sx,sy;

		Bitmap^ dbmp;
		BitmapData^ dbmpdt;
		array<Byte>^ ddt;
		double dw,dh;
		int dx,dy;

		int lstype;
		int lsavlbl;
		array<double,2>^ lscth;
		array<double,2>^ lssrad;
		array<double,2>^ lscrad;
		double plt,plg;
		bool dirEW;
		double splt,cplt,cth,srad,crad,dlt,dlg;

		Point mspt; //mouse
		Point dmspt; //mouse

		Thread^ thd;
		bool enthd;
		bool bsythd;
		 bool ordthd;
		 
		 double magn;


private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
private: System::Windows::Forms::ToolStripMenuItem^  equidistantToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  equalareaToolStripMenuItem;
private: System::Windows::Forms::MenuStrip^  menuStrip1;
private: System::Windows::Forms::ToolStripMenuItem^  settingsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  tyepOfMapToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  sourceImageToolStripMenuItem;


private: System::Windows::Forms::VScrollBar^  vScrollBar1;

	private:
		void thdfunc(){
			while(this->enthd){
				if(this->ordthd){
					this->bsythd = true;
					while(this->ordthd == true){Thread::Sleep(10);}
					while(this->lsavlbl == false){Thread::Sleep(10);}
					mainfunc();
					if(this->enthd == true)
						Invoke(gcnew updtimgl(this,&Form1::updtimg));
					this->bsythd = false;
				}
			}
		}

	private:
		void updtsrcimg(Bitmap^ bmp){
			sbmp = bmp;
			sw = sbmp->Width;
			sh = sbmp->Height;
			sdt = gcnew array<Byte>((int)(sw*sh*3));
			sbmpdt = sbmp->LockBits(
				Rectangle(Point::Empty, sbmp->Size),
				ImageLockMode::ReadOnly,
				PixelFormat::Format24bppRgb);
			Marshal::Copy(sbmpdt->Scan0,sdt,0,(int)(sw*sh*3));
			sbmp->UnlockBits(sbmpdt);
		}
	private:
		void updtls(){
			double leng, rad;

			dx = 0;
			while(dx < dw){
				dy = 0;
				while(dy < dh){
					leng = sqrt(pow(dw/2-dx,2)+pow(dh/2-dy,2));
					if(this->lstype == 0){
						rad = leng/(dw/2)*PI*this->magn;
					}else{
						rad = (PI - 2*acos(leng*this->magn/(dw/2)));
						if(rad < 0)
							rad = 0;
					}

					if(leng > dw/2){
						lssrad[dx,dy] = -1;
						lscrad[dx,dy] = -1;
						lscth[dx,dy] = -1;
					}else{
						lssrad[dx,dy] = sin(rad);
						lscrad[dx,dy] = cos(rad);
						if(leng == 0)
							lscth[dx,dy] = 1;
						else
							lscth[dx,dy] = (dh/2-dy)/leng;
					}
					dy++;
				}
				dx++;
			}

		}

	private:
		void mainfunc(){

			splt = sin(plt);
			cplt = cos(plt);

			dx = 0;
			while(dx < dw){
				dy = 0;
				while(dy < dh){
					if(lssrad[dx,dy] == -1){
						ddt[(int)(dx*3+dy*3*dw)] = (Byte)0;
						ddt[(int)(dx*3+dy*3*dw)+1] = (Byte)0;
						ddt[(int)(dx*3+dy*3*dw)+2] = (Byte)0;
					}else{
						if(dx >= (int)(dw/2))
							dirEW = true;
						else
							dirEW = false;
						cth = lscth[dx,dy];
						srad = lssrad[dx,dy];
						crad = lscrad[dx,dy];
						azi_calc(dirEW,splt,cplt,cth,srad,crad,dlt,dlg);
						dlg = dlg + plg;
						if(dlg > PI)
							dlg = dlg - 2*PI;
						else if(dlg < -1*PI)
							dlg = dlg + 2*PI;
						sx = (int)((dlg+PI)/(2*PI)*(sw-1));
						sy = (int)(dlt/PI*(sh-1));

						ddt[(int)(dx*3+dy*3*dw)] = sdt[(int)(sx*3+sy*3*sw)];
						ddt[(int)(dx*3+dy*3*dw)+1] = sdt[(int)(sx*3+sy*3*sw)+1];
						ddt[(int)(dx*3+dy*3*dw)+2] = sdt[(int)(sx*3+sy*3*sw)+2];
					}
					dy++;
				}
				dx++;
			}			

		}
		delegate void updtimgl();
		void updtimg(){
			dbmpdt = dbmp->LockBits(
				Rectangle(Point::Empty, dbmp->Size),
				ImageLockMode::WriteOnly,
				PixelFormat::Format24bppRgb);
			Marshal::Copy(ddt,0,dbmpdt->Scan0,(int)(dw*dh*3));
			dbmp->UnlockBits(dbmpdt);
			this->pictureBox1->Image = dbmp;
		}

	private: System::Windows::Forms::PictureBox^  pictureBox1;
private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->equidistantToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->equalareaToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->vScrollBar1 = (gcnew System::Windows::Forms::VScrollBar());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tyepOfMapToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->sourceImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->contextMenuStrip1->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(0, 28);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(400, 400);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseMove);
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseDown);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->equidistantToolStripMenuItem, 
				this->equalareaToolStripMenuItem});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(141, 48);
			// 
			// equidistantToolStripMenuItem
			// 
			this->equidistantToolStripMenuItem->Name = L"equidistantToolStripMenuItem";
			this->equidistantToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->equidistantToolStripMenuItem->Text = L"equidistant";
			this->equidistantToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::equidistantToolStripMenuItem_Click);
			// 
			// equalareaToolStripMenuItem
			// 
			this->equalareaToolStripMenuItem->Name = L"equalareaToolStripMenuItem";
			this->equalareaToolStripMenuItem->Size = System::Drawing::Size(140, 22);
			this->equalareaToolStripMenuItem->Text = L"equal-area";
			this->equalareaToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::equalareaToolStripMenuItem_Click);
			// 
			// vScrollBar1
			// 
			this->vScrollBar1->Location = System::Drawing::Point(403, 26);
			this->vScrollBar1->Name = L"vScrollBar1";
			this->vScrollBar1->Size = System::Drawing::Size(22, 400);
			this->vScrollBar1->TabIndex = 1;
			this->vScrollBar1->ValueChanged += gcnew System::EventHandler(this, &Form1::vScrollBar1_ValueChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->settingsToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(426, 26);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// settingsToolStripMenuItem
			// 
			this->settingsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->tyepOfMapToolStripMenuItem, 
				this->sourceImageToolStripMenuItem});
			this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
			this->settingsToolStripMenuItem->Size = System::Drawing::Size(66, 22);
			this->settingsToolStripMenuItem->Text = L"settings";
			// 
			// tyepOfMapToolStripMenuItem
			// 
			this->tyepOfMapToolStripMenuItem->Name = L"tyepOfMapToolStripMenuItem";
			this->tyepOfMapToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->tyepOfMapToolStripMenuItem->Text = L"tyep of map";
			this->tyepOfMapToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::tyepOfMapToolStripMenuItem_Click);
			// 
			// sourceImageToolStripMenuItem
			// 
			this->sourceImageToolStripMenuItem->Name = L"sourceImageToolStripMenuItem";
			this->sourceImageToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->sourceImageToolStripMenuItem->Text = L"source image";
			this->sourceImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sourceImageToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(426, 431);
			this->ContextMenuStrip = this->contextMenuStrip1;
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->vScrollBar1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Azimuthal map";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->contextMenuStrip1->ResumeLayout(false);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	 private: System::Void pictureBox1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e){
				  if((e->Button & System::Windows::Forms::MouseButtons::Left) == System::Windows::Forms::MouseButtons::Left){
					  mspt.X = e->X;
					  mspt.Y = e->Y;
				  }
			  }
private: System::Void pictureBox1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			 if((e->Button & System::Windows::Forms::MouseButtons::Left) == System::Windows::Forms::MouseButtons::Left){
				if(this->bsythd == false){
					dmspt.X = (mspt.X - e->X);//東方向の移動量
					dmspt.Y = (mspt.Y - e->Y);//南方向の移動量
					mspt.X = e->X;
					mspt.Y = e->Y;
					
					plg = dmspt.X * this->magn / dw * PI + plg;
					if(plg > PI)
						plg = plg - 2*PI;
					else if(plg <= -1*PI)
						plg = plg + 2*PI;
					plt = dmspt.Y * this->magn / dh * PI + plt;
					if(plt > PI)
						plt = PI;
					else if(plt < 0)
						plt = 0;

					this->ordthd = true;
					while(this->bsythd == false){ Thread::Sleep (10); }
					this->ordthd = false;
					
				} 
			}
		 }

private: System::Void equidistantToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 while(this->bsythd){}
			 this->lsavlbl = false;
			 this->lstype = 0;
			 this->equidistantToolStripMenuItem->Checked = true;
			 this->equalareaToolStripMenuItem->Checked = false;
			 updtls();
			 mainfunc();
			 updtimg();
			 this->lsavlbl = true;
		 }
private: System::Void equalareaToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 while(this->bsythd){}
			 this->lsavlbl = false;
			 this->lstype = 1;
			 this->equidistantToolStripMenuItem->Checked = false;
			 this->equalareaToolStripMenuItem->Checked = true;
			 updtls();
			 mainfunc();
			 updtimg();
			 this->lsavlbl = true;
		 }
private: System::Void vScrollBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 while(this->bsythd){}
			 this->lsavlbl = false;
			 this->magn = (double)(this->vScrollBar1->Maximum - this->vScrollBar1->Value)/this->vScrollBar1->Maximum;
			 updtls();
			 mainfunc();
			 updtimg();
			 this->lsavlbl = true;
		 }
private: System::Void tyepOfMapToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 TPSel^ fm = gcnew TPSel(this->lstype);
			 fm->ShowDialog();
			 int type = fm->getType();
			 if(this->lstype != type){
				 this->lsavlbl = false;
				 this->lstype = type;
				 if(this->lstype == 0){
					 this->equidistantToolStripMenuItem->Checked = true;
					 this->equalareaToolStripMenuItem->Checked = false;
				 }else if(this->lstype == 1){
					 this->equidistantToolStripMenuItem->Checked = false;
					 this->equalareaToolStripMenuItem->Checked = true;
				 }else{
					 this->equidistantToolStripMenuItem->Checked = true;
					 this->equalareaToolStripMenuItem->Checked = false;
				 }
				 updtls();
				 mainfunc();
				 updtimg();
				 this->lsavlbl = true;
			 }		 
		 }
private: System::Void sourceImageToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 ChSrc^ fm = gcnew ChSrc(this->srcimg);
			 fm->ShowDialog();
			 String^ src = fm->getSrcImg();
			 if(this->srcimg->Equals(src) == false){
				 this->lsavlbl = false;
				 this->srcimg = src;
				 if(this->srcimg->Equals("")){
					 Assembly^ asmbly = Assembly::GetExecutingAssembly();
					 updtsrcimg(gcnew Bitmap(asmbly->GetManifestResourceStream("source.jpg")));
				 }else{
					 Bitmap^ bmp;
					 try{
						 bmp = gcnew Bitmap(this->srcimg);
						 if(bmp->Width != 2*bmp->Height){
							 this->srcimg = "";
							 Assembly^ asmbly = Assembly::GetExecutingAssembly();
							 bmp = gcnew Bitmap(asmbly->GetManifestResourceStream("source.jpg"));
						 }
					 }catch(...){
						 this->srcimg = "";
						 Assembly^ asmbly = Assembly::GetExecutingAssembly();
						 bmp = gcnew Bitmap(asmbly->GetManifestResourceStream("source.jpg"));
					 }
					 updtsrcimg(bmp);
				 }
				 updtls();
				 mainfunc();
				 updtimg();
				 this->lsavlbl = true;
			 }
		 }
};
}

