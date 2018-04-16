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
	/// XGLControl ժҪ
	/// </summary>
	public ref class XGLControl : public System::Windows::Forms::UserControl
	{
	public:
		XGLControl(void)
		{
			InitializeComponent();
			//
			//TODO:  �ڴ˴���ӹ��캯������
			//
		}
		void BindXGLInterface(intptr_t viewer);
		void Render();
		void Setup(String^ name);
		int Close();
	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
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
		/// ����������������
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����֧������ķ��� - ��Ҫ�޸�
		/// ʹ�ô���༭���޸Ĵ˷��������ݡ�
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
