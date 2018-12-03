#pragma once

#define FST_N (0)
#define FST_E (0)

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
			dbmp = nullptr;
			ddt = nullptr;
			lscth = nullptr;
			lssrad = nullptr;
			lscrad = nullptr;
			this->chgWinSiz(533);

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
		‾Form1()
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
		int spad;

		Bitmap^ dbmp;
		BitmapData^ dbmpdt;
		array<Byte>^ ddt;
		double dw,dh;
		int dx,dy;
		int dpad;

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
private: System::Windows::Forms::ToolStripMenuItem^  windowSizeToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  smallToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  middleToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  largeToolStripMenuItem;


private: System::Windows::Forms::VScrollBar^  vScrollBar1;
	private:
		// to change window size
		void chgWinSiz(int size){
			this->pictureBox1->Width = size; this->pictureBox1->Height = size;
			this->Width =  this->pictureBox1->Width + 50;
			this->Height = this->pictureBox1->Height + 90;
			if (dbmp != nullptr)
				delete dbmp;
			dbmp = gcnew Bitmap(this->pictureBox1->Width, this->pictureBox1->Height);
			dw = dbmp->Width;
			dh = dbmp->Height;
			dpad = (4 - ((int)(3 * dw) % 4)) % 4;
			if (dbmp != nullptr)
				delete ddt;
			ddt = gcnew array<Byte>((int)((3 * dw + dpad)*dh));
			for (int dy = 0; dy < dh; dy++){
				for (int dx = 0; dx < dw; dx++){
					ddt[(int)(dx * 3 + dy * (3 * dw + dpad))] = (Byte)0;
					ddt[(int)(dx * 3 + dy * (3 * dw + dpad)) + 1] = (Byte)0;
					ddt[(int)(dx * 3 + dy * (3 * dw + dpad)) + 2] = (Byte)0;
				}
			}
			if (lscth != nullptr)
				delete lscth;
			lscth = gcnew array<double, 2>((int)dw, (int)dh);
			if (lssrad != nullptr)
				delete lssrad;
			lssrad = gcnew array<double, 2>((int)dw, (int)dh);
			if (lscrad != nullptr)
				delete lscrad;
			lscrad = gcnew array<double, 2>((int)dw, (int)dh);

			updtls();

		}
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
			spad = (4 - ((int)(3*sw) % 4)) % 4;
			sdt = gcnew array<Byte>((int)((sw+spad)*sh*3));
			sbmpdt = sbmp->LockBits(
				Rectangle(Point::Empty, sbmp->Size),
				ImageLockMode::ReadOnly,
				PixelFormat::Format24bppRgb);
			Marshal::Copy(sbmpdt->Scan0,sdt,0,(int)((3*sw+spad)*sh));
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
		// a function to map pixels from the source image to the display
		// the mapping is based on the macro "azi_calc"
		void mainfunc(){

			splt = sin(plt);
			cplt = cos(plt);

			dx = 0;
			while(dx < dw){
				dy = 0;
				while(dy < dh){
					if(lssrad[dx,dy] != -1){ // inside the circle
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

						ddt[(int)(dx * 3 + dy * (3 * dw + dpad))] = sdt[(int)(sx * 3 + sy * (3 * sw + spad))];
						ddt[(int)(dx * 3 + dy * (3 * dw + dpad)) + 1] = sdt[(int)(sx * 3 + sy * (3 * sw + spad)) + 1];
						ddt[(int)(dx * 3 + dy * (3 * dw + dpad)) + 2] = sdt[(int)(sx * 3 + sy * (3 * sw + spad)) + 2];
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
			Marshal::Copy(ddt, 0, dbmpdt->Scan0, (int)((3 * dw + dpad)*dh));
			dbmp->UnlockBits(dbmpdt);
			this->pictureBox1->Image = dbmp;
		}

		// ***these are functions for check of bitmap file***
		// save binary data to a file in bitmap format
		void saveBitmap24(array<Byte>^ data, int w, int h, String^ filename){
			saveBitmap24(data, w, h, filename, false,false);
		}
		void saveBitmap24(array<Byte>^ data, int w, int h, String^ filename, bool bgr){
			saveBitmap24(data, w, h, filename, bgr, false);
		}
		void saveBitmap24(array<Byte>^ data, int w, int h, String^ filename, bool bgr /*if source data has BGR order */, bool pad /*if data row has padding*/){
			// create an array holding image data based on Bitmap24
			int filesize;
			array<Byte>^ filedata = convByte2Bitmap24(data, w, h, &filesize,bgr,pad);

			// write data on the memory to a file
			System::IO::FileStream^ fileStream = gcnew System::IO::FileStream(filename, System::IO::FileMode::Create);
			fileStream->Write(filedata, 0, filesize);
			fileStream->Close();
		}

		// create byte array holding bitmap data, given binary data
		array<Byte>^ convByte2Bitmap24(array<Byte>^ data, int w, int h, int* filesize, bool bgr /*if source data has BGR order */, bool pad /*if data row has padding*/){
			// calculate sizes of each data chunk
			int hsize = 14; // header size
			int dsize = 40; // DIB header size
			int off2pixel = hsize + dsize; // offset to pixelarray
			int psize = (4 - ((3*w)%4))%4; // padding size
			int isize = (w*3+psize)*h; // image size
			int fsize = off2pixel + isize; // file size

			// create memory space
			array<Byte>^ filedata = gcnew array<Byte>(fsize);

			// set header
			filedata[0] = 'B'; filedata[1] = 'M'; // signature
			int curp = 2; // current point on the mem
			curp = setDT_LE(filedata, fsize, curp, 4); // file size
			curp = setDT_LE(filedata, 0, curp, 4); // reserved x 4 bytes
			curp = setDT_LE(filedata, off2pixel, curp, 4); // file offset to pixel array

			// set image information
			curp = setDT_LE(filedata, dsize, curp, 4); // DIB Header Size 4
			curp = setDT_LE(filedata, w, curp, 4); // width 4
			curp = setDT_LE(filedata, h, curp, 4); // height 4
			curp = setDT_LE(filedata, 1, curp, 2); // planes 2
			curp = setDT_LE(filedata, 24, curp, 2); // bits per pixel 2
			curp = setDT_LE(filedata, 0, curp, 4); // compression 4
			curp = setDT_LE(filedata, isize, curp, 4); // image size 4
			curp = setDT_LE(filedata, 0, curp, 4); // x pixels per meter 4
			curp = setDT_LE(filedata, 0, curp, 4); // y pixels per meter 4
			curp = setDT_LE(filedata, 0, curp, 4); // colors in color table 4
			curp = setDT_LE(filedata, 0, curp, 4); // important color count 4

			// set data
			int padding = 0;
			if (pad)
				padding = psize;
			for (int ih = 0; ih < h; ih++){
				for (int iw = 0; iw < w; iw++){
					if (bgr){
						filedata[curp + ih * (3 * w + psize) + 3 * iw] = data[(h - ih - 1) * (3 * w + padding) + 3 * iw]; //R
						filedata[curp + ih * (3 * w + psize) + 3 * iw + 1] = data[(h - ih - 1) * (3 * w + padding) + 3 * iw + 1]; //G
						filedata[curp + ih * (3 * w + psize) + 3 * iw + 2] = data[(h - ih - 1) * (3 * w + padding) + 3 * iw + 2]; //B
					}
					else{
						filedata[curp + ih * (3 * w + psize) + 3 * iw + 2] = data[(h - ih - 1) * (3 * w + padding) + 3 * iw]; //R
						filedata[curp + ih * (3 * w + psize) + 3 * iw + 1] = data[(h - ih - 1) * (3 * w + padding) + 3 * iw + 1]; //G
						filedata[curp + ih * (3 * w + psize) + 3 * iw] = data[(h - ih - 1) * (3 * w + padding) + 3 * iw + 2]; //B
					}
				}
				for (int i = 0; i < psize; i++){
					filedata[curp + (ih + 1) * (3 * w + psize) - i - 1] = 0x00;
				}
			}

			*filesize = fsize;

			return filedata;
		}

		int setDT_LE(array<Byte>^ dest, int value, int offset, int size){
			for (int i = 0; i < size; i++){
				dest[offset + i] = (unsigned char)(value >> (i * 8));
			}
			return offset + size;
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
			this->windowSizeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->smallToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->middleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->largeToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->contextMenuStrip1->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(0, 35);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(4);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(533, 533);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseDown);
			this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseMove);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->equidistantToolStripMenuItem,
					this->equalareaToolStripMenuItem
			});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(153, 52);
			// 
			// equidistantToolStripMenuItem
			// 
			this->equidistantToolStripMenuItem->Name = L"equidistantToolStripMenuItem";
			this->equidistantToolStripMenuItem->Size = System::Drawing::Size(152, 24);
			this->equidistantToolStripMenuItem->Text = L"equidistant";
			this->equidistantToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::equidistantToolStripMenuItem_Click);
			// 
			// equalareaToolStripMenuItem
			// 
			this->equalareaToolStripMenuItem->Name = L"equalareaToolStripMenuItem";
			this->equalareaToolStripMenuItem->Size = System::Drawing::Size(152, 24);
			this->equalareaToolStripMenuItem->Text = L"equal-area";
			this->equalareaToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::equalareaToolStripMenuItem_Click);
			// 
			// vScrollBar1
			// 
			this->vScrollBar1->Dock = System::Windows::Forms::DockStyle::Right;
			this->vScrollBar1->Location = System::Drawing::Point(539, 0);
			this->vScrollBar1->Name = L"vScrollBar1";
			this->vScrollBar1->Size = System::Drawing::Size(22, 572);
			this->vScrollBar1->TabIndex = 1;
			this->vScrollBar1->ValueChanged += gcnew System::EventHandler(this, &Form1::vScrollBar1_ValueChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->settingsToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 3, 0, 3);
			this->menuStrip1->Size = System::Drawing::Size(539, 30);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// settingsToolStripMenuItem
			// 
			this->settingsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->tyepOfMapToolStripMenuItem,
					this->sourceImageToolStripMenuItem, this->windowSizeToolStripMenuItem
			});
			this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
			this->settingsToolStripMenuItem->Size = System::Drawing::Size(72, 24);
			this->settingsToolStripMenuItem->Text = L"settings";
			// 
			// tyepOfMapToolStripMenuItem
			// 
			this->tyepOfMapToolStripMenuItem->Name = L"tyepOfMapToolStripMenuItem";
			this->tyepOfMapToolStripMenuItem->Size = System::Drawing::Size(167, 24);
			this->tyepOfMapToolStripMenuItem->Text = L"type of map";
			this->tyepOfMapToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::tyepOfMapToolStripMenuItem_Click);
			// 
			// sourceImageToolStripMenuItem
			// 
			this->sourceImageToolStripMenuItem->Name = L"sourceImageToolStripMenuItem";
			this->sourceImageToolStripMenuItem->Size = System::Drawing::Size(167, 24);
			this->sourceImageToolStripMenuItem->Text = L"source image";
			this->sourceImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sourceImageToolStripMenuItem_Click);
			// 
			// windowSizeToolStripMenuItem
			// 
			this->windowSizeToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->smallToolStripMenuItem,
					this->middleToolStripMenuItem, this->largeToolStripMenuItem
			});
			this->windowSizeToolStripMenuItem->Name = L"windowSizeToolStripMenuItem";
			this->windowSizeToolStripMenuItem->Size = System::Drawing::Size(167, 24);
			this->windowSizeToolStripMenuItem->Text = L"window size";
			// 
			// smallToolStripMenuItem
			// 
			this->smallToolStripMenuItem->Name = L"smallToolStripMenuItem";
			this->smallToolStripMenuItem->Size = System::Drawing::Size(133, 24);
			this->smallToolStripMenuItem->Text = L"small";
			this->smallToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::smallToolStripMenuItem_Click);
			// 
			// middleToolStripMenuItem
			// 
			this->middleToolStripMenuItem->Name = L"middleToolStripMenuItem";
			this->middleToolStripMenuItem->Size = System::Drawing::Size(133, 24);
			this->middleToolStripMenuItem->Text = L"medium";
			this->middleToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::middleToolStripMenuItem_Click);
			// 
			// largeToolStripMenuItem
			// 
			this->largeToolStripMenuItem->Name = L"largeToolStripMenuItem";
			this->largeToolStripMenuItem->Size = System::Drawing::Size(133, 24);
			this->largeToolStripMenuItem->Text = L"large";
			this->largeToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::largeToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
			this->ClientSize = System::Drawing::Size(561, 572);
			this->ContextMenuStrip = this->contextMenuStrip1;
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->vScrollBar1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"Form1";
			this->Text = L"Azimuthal map";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
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
					 updtls();
					 mainfunc();
					 updtimg();
				 }else{
					 Bitmap^ bmp;
					 try{
						 bmp = gcnew Bitmap(this->srcimg);
						 if (bmp->Width == 2 * bmp->Height){
							 updtsrcimg(bmp);
							 updtls();
							 mainfunc();
							 updtimg();
						 }else{
							 MessageBox::Show("The image ratio (width, height) needs to be 2:1.", "wrong image ratio", MessageBoxButtons::OK, MessageBoxIcon::Warning);
						 }
					 }catch(...){
						 this->srcimg = "";
						 MessageBox::Show("The file format is wrong.", "wrong image format", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					 }
				 }
				 this->lsavlbl = true;
			 }
		 }
// === window size ===
// small
private: System::Void smallToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 while (this->bsythd){}
			 this->lsavlbl = false;
			 this->chgWinSiz(333);
			 updtls();
			 mainfunc();
			 updtimg();
			 this->lsavlbl = true;
}
// medium
private: System::Void middleToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 while (this->bsythd){}
			 this->lsavlbl = false;
			 this->chgWinSiz(533);
			 updtls();
			 mainfunc();
			 updtimg();
			 this->lsavlbl = true;
}
// large
private: System::Void largeToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 while (this->bsythd){}
			 this->lsavlbl = false;
			 this->chgWinSiz(733);
			 updtls();
			 mainfunc();
			 updtimg();
			 this->lsavlbl = true;
}
};
}

