#pragma once
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace XGLInterface {
	class XGLInterface;
}
namespace XGLWrapper {

	/// <summary>
	/// XGLControl 摘要
	/// </summary>
	public ref class XGLControl : public System::Windows::Forms::UserControl
	{
	public:
		XGLControl(void)
		{
			InitializeComponent();
			//
			//TODO:  在此处添加构造函数代码
			//
		}
		void BindXGLInterface(intptr_t viewer);
		void Render();
		void Setup(String^ name);
		int Close();
	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~XGLControl()
		{
			if (components)
			{
				delete components;
			}
			this->!XGLControl();
		}
		!XGLControl();

		virtual void WndProc(System::Windows::Forms::Message% m)override;

	private:
		bool dispatchMessage(System::Windows::Forms::Message% m);

	private:
		XGLInterface::XGLInterface* xgl;
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要修改
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent(void)
		{
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::LightSkyBlue;
			this->Name = L"XGLControl";

		}
#pragma endregion
	};
}
