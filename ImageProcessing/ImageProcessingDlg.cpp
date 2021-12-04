
// ImageProcessingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "ImageProcessingDlg.h"
#include "afxdialogex.h"
#include "ImageHandler.h"
#include "CStaticHandler.h"
#include "PPM_Reader.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
// ImageProcessingDlg dialog
ImageProcessingDlg::ImageProcessingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ImageProcessing_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void ImageProcessingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_input_frame, input_frame);
	DDX_Control(pDX, IDC_output_frame, output_frame);
	DDX_Control(pDX, IDC_ROTATE_NUMBER, rotate_number);
	DDX_Control(pDX, IDC_RESIZE_NUMBER, resize_number);
	DDX_Control(pDX, IDC_TRANSLATION_NUMBER_X, translation_number_x);
	DDX_Control(pDX, IDC_TRANSLATION_NUMBER_Y, translation_number_y);
	DDX_Control(pDX, IDC_SMOOTH_ALGO, smooth_algorithm);
	DDX_Control(pDX, IDC_EDGE_DETECT_ALGO, edge_detect_algorithm);
	DDX_Control(pDX, IDC_GAMMA_VALUE, gamma);
	DDX_Control(pDX, IDC_WAVE_COEFF, wave_coeff);
	DDX_Control(pDX, IDC_WAVE_THETA_COEFF, wave_theta_coeff);
	DDX_Control(pDX, IDC_WAVE_HORIZONTAL_SHIFT, wave_hor_shift);
	DDX_Control(pDX, IDC_WAVE_VERTICAL_SHIFT, wave_ver_shift);
	DDX_Control(pDX, IDC_SUBCOLOR_RANGE, subColorRange);
	DDX_Control(pDX, IDC_SUBCOLOR_SELECTOR, subColorType);

}
BEGIN_MESSAGE_MAP(ImageProcessingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD, &ImageProcessingDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_SAVE, &ImageProcessingDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_RESIZE, &ImageProcessingDlg::OnBnClickedResize)
	ON_BN_CLICKED(IDC_Translation, &ImageProcessingDlg::OnBnClickedTranslation)
	ON_BN_CLICKED(IDC_CANCEL, &ImageProcessingDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ROTATE, &ImageProcessingDlg::OnBnClickedRotate)
	ON_BN_CLICKED(IDC_OUTPUT_AS_INPUT, &ImageProcessingDlg::OnBnClickedOutputAsInput)
	ON_BN_CLICKED(IDC_GRAY, &ImageProcessingDlg::OnBnClickedGray)
	ON_BN_CLICKED(IDC_GRAY_HIST, &ImageProcessingDlg::OnBnClickedGrayHist)
	ON_BN_CLICKED(IDC_SMOOTH, &ImageProcessingDlg::OnBnClickedSmooth)
	ON_BN_CLICKED(IDC_EDGE_DETECTION, &ImageProcessingDlg::OnBnClickedEdgeDetection)
	ON_BN_CLICKED(IDC_HALF_SIZE, &ImageProcessingDlg::OnBnClickedHalfSize)
	ON_BN_CLICKED(IDC_FourierTransform, &ImageProcessingDlg::OnBnClickedFouriertransform)
	ON_BN_CLICKED(IDC_DOUBLE_SIZE, &ImageProcessingDlg::OnBnClickedDoubleSize)
	ON_BN_CLICKED(IDC_GAMMA, &ImageProcessingDlg::OnBnClickedGamma)
	ON_BN_CLICKED(IDC_WAVE, &ImageProcessingDlg::OnBnClickedWave)
	ON_BN_CLICKED(IDC_SUBCOLOR, &ImageProcessingDlg::OnBnClickedSubcolor)
	ON_BN_CLICKED(IDC_DirectWave_FFT, &ImageProcessingDlg::OnBnClickedDirectwaveFft)
	ON_BN_CLICKED(IDC_Fourier_HighPass_Inverse, &ImageProcessingDlg::OnBnClickedFourierHighpassInverse)
	ON_BN_CLICKED(IDC_TOON_SHADER, &ImageProcessingDlg::OnBnClickedToonShader)
END_MESSAGE_MAP()
// ImageProcessingDlg message handlers
BOOL ImageProcessingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	width = CStaticHandler::getCStaticWidth(&output_frame);
	height = CStaticHandler::getCStaticHeight(&output_frame);
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void ImageProcessingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void ImageProcessingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ImageProcessingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void ImageProcessingDlg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialogEx::OnOK();
}
void ImageProcessingDlg::OnBnClickedLoad()
{
	if (!input_image.IsNull())
	{
		input_image.Destroy();
	}
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	//CFileDialog dlg(TRUE);
	CString szFilter = _T("BMP Files (*.bmp)|*.bmp|JPEG(.jpeg)|*.jpeg;*.jpg|PPM(*.ppm)|*.ppm|PNG(*.png)|*.png|GIF(*.gif)|*.gif|");
	CFileDialog dlg(true, NULL, NULL, OFN_HIDEREADONLY, szFilter, NULL);
	dlg.m_ofn.nMaxFile = 1;
	string filename = "";
	string filepath;
	string extension;
	string fileWholePath = "";

	if (dlg.DoModal() == IDOK)
	{
		filepath.append(CT2CA(dlg.GetFolderPath()));
		string temp = CT2CA(dlg.GetFileName());
		size_t dotPosition = temp.rfind(".");
		filename = temp.substr(0, dotPosition).c_str();
		extension = temp.substr(dotPosition + 1, temp.length());
		fileWholePath.append(filepath);
		fileWholePath.append("\\");
		fileWholePath.append(filename);
		fileWholePath.append(".");
		fileWholePath.append(extension);
	}
	PPM_Reader reader;
	bool readSuccss = false;
	CImage buffer;
	if (reader.isPPM(extension))
	{
		if (reader.readPPM(filepath, filename, extension))
		{
			buffer = *reader.toCImage();
			readSuccss = true;
		}
	}
	else
	{
		if (buffer.Load(CString(fileWholePath.c_str())) == S_OK)
		{
			readSuccss = true;
		}
	}
	if (readSuccss)
	{

		if (CImageCopy(&output_image, &buffer) && CImageCopy(&input_image, &buffer))
		{
			CStaticHandler::SetBitmap(&input_frame, &input_image_to_show, &input_image);
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
		else
		{
			CStaticHandler::setBlankDefault(&input_frame, width, height, 32);
			CStaticHandler::setBlankDefault(&output_frame, width, height, 32);
			input_image.Destroy();
			output_image.Destroy();
		}
	}
}
void ImageProcessingDlg::OnBnClickedSave()
{
	if (!output_image.IsNull())
	{
		CString szFilter = _T("BMP Files (*.bmp)|*.bmp||");
		CFileDialog dlg(false, NULL, NULL, OFN_HIDEREADONLY, szFilter, NULL);
		dlg.m_ofn.nMaxFile = 1;
		CString filename;
		if (dlg.DoModal() == IDOK)
		{
			filename = dlg.GetFolderPath();// return full path and filename
			filename.Append(_T("\\"));
			filename.Append(dlg.GetFileName());
			filename.Append(_T(".bmp"));
		}
		output_image.Save(filename, Gdiplus::ImageFormatBMP);
	}

}
void ImageProcessingDlg::OnBnClickedResize()
{

	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		CString strSource;
		UpdateData(true);
		resize_number.GetWindowText(strSource);
		int resize = _ttoi(strSource);
		if (0 < resize && resize <= 30)
		{
			CImage temp = *downScale(&input_image, resize);
			output_image = *upScale(&temp, resize);
			temp.Destroy();
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
void ImageProcessingDlg::OnBnClickedTranslation()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		CString x, y;
		//UpdateData(true);
		translation_number_x.GetWindowText(x);
		translation_number_y.GetWindowText(y);
		int x_num = _ttoi(x);
		int y_num = _ttoi(y);
		CImage temp;
		if (translation(&output_image, &input_image, x_num, y_num))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}

	}
}
void ImageProcessingDlg::OnBnClickedCancel()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CDialogEx::OnCancel();
}
void ImageProcessingDlg::OnBnClickedRotate()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		CString strSource;
		//UpdateData(true);
		rotate_number.GetWindowText(strSource);
		int resize = _ttoi(strSource);
		if (rotate(&output_image, &input_image, resize))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}

	}
}
void ImageProcessingDlg::OnBnClickedOutputAsInput()
{
	CImage temp;
	if (CImageCopy(&input_image, &output_image))
	{
		CStaticHandler::SetResizeBitmap(&input_frame, &input_image_to_show, &input_image, input_image.GetWidth(), input_image.GetHeight());
	}
	// TODO: 在此加入控制項告知處理常式程式碼
}
void ImageProcessingDlg::OnBnClickedGray()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{

		if (toGray(&output_image, &input_image))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
void ImageProcessingDlg::OnBnClickedGrayHist()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		if (grayIntensityHistogram(&output_image, &input_image))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
void ImageProcessingDlg::OnBnClickedSmooth()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		double atom = (double)1 / 9;
		int algo = smooth_algorithm.GetCurSel();
		double mask[9] = { 0 };
		//weighted average filter
		if (algo == 1)
		{
			int atom[] = { 1,2,1 };
			for (int w = 0; w < 3; w++)
			{
				for (int h = 0; h < 3; h++)
				{
					mask[h * 3 + w] = (double)(atom[w] * atom[h]) / 16;
				}
			}
		}
		//box filter
		else
		{
			for (int i = 0; i < 9; i++)
			{
				mask[i] = atom;
			}
		}

		if (filter(&output_image, &input_image, mask, 3, 3))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
void ImageProcessingDlg::OnBnClickedEdgeDetection()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{

		int algo = edge_detect_algorithm.GetCurSel();
		double mask[9] = { 0 };
		if (algo == 0)
		{
			int base[] = { 1,2,1 };
			for (int i = 0; i < 3; i++)
			{
				mask[i] = base[i];
				mask[i + 6] = -base[i];
			}
		}
		else if (algo == 1)
		{
			int base[] = { 1,0,-1 };
			int sign[] = { 1, 2, 1 };
			for (int h = 0; h < 3; h++)
			{
				for (int w = 0; w < 3; w++)
				{
					mask[3 * h + w] = base[w] * sign[h];
				}
			}
		}
		else
		{
			for (int i = 0; i < 9; i++)
			{
				mask[i] = 1;
			}
			mask[4] = -8;
		}

		if (filter(&output_image, &input_image, mask, 3, 3))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
void ImageProcessingDlg::OnBnClickedHalfSize()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		if (downScale(&output_image, &input_image, 2))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
void ImageProcessingDlg::OnBnClickedFouriertransform()
{

	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		if (fourierTransform(&output_image, &input_image))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
void ImageProcessingDlg::OnBnClickedDoubleSize()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		if (upScale(&output_image, &input_image, 2))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
void ImageProcessingDlg::OnBnClickedGamma()
{
	CString strSource;
	//UpdateData(true);
	gamma.GetWindowText(strSource);
	double gammaValue = _ttof(strSource);
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		if (gammaCorrection(&output_image, &input_image, gammaValue))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}


void ImageProcessingDlg::OnBnClickedWave()
{
	CString coeff, hor_shift, ver_shift, theta_coeff;
	//coeff--------------------------
	wave_coeff.GetWindowText(coeff);
	double wave_coeff = _ttof(coeff);
	//-------------------------------
	//coeff--------------------------
	wave_hor_shift.GetWindowText(hor_shift);
	double hor_shift_ = _ttof(hor_shift);
	//-------------------------------
	//coeff--------------------------
	wave_ver_shift.GetWindowText(ver_shift);
	double ver_shift_ = _ttof(ver_shift);
	//-------------------------------
	//coeff--------------------------
	wave_theta_coeff.GetWindowText(theta_coeff);
	double theta_coeff_ = _ttof(theta_coeff);
	//-------------------------------

	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (wave(&output_image, 255, 255, wave_coeff, hor_shift_, ver_shift_, theta_coeff_))
	{
		CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
	}

}



void ImageProcessingDlg::OnBnClickedSubcolor()
{
	CString subColorRange_;
	this->subColorRange.GetWindowText(subColorRange_);
	double subColorRange = _ttof(subColorRange_);
	COLORREF color = this->subColorType.GetColor();
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		if (toSubColor(&output_image, &input_image, (int)subColorRange, color))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}

}





void ImageProcessingDlg::OnBnClickedDirectwaveFft()
{
	CString coeff, hor_shift, ver_shift, theta_coeff;
	//coeff--------------------------
	wave_coeff.GetWindowText(coeff);
	double wave_coeff = _ttof(coeff);
	//-------------------------------
	//coeff--------------------------
	wave_hor_shift.GetWindowText(hor_shift);
	double hor_shift_ = _ttof(hor_shift);
	//-------------------------------
	//coeff--------------------------
	wave_ver_shift.GetWindowText(ver_shift);
	double ver_shift_ = _ttof(ver_shift);
	//-------------------------------
	//coeff--------------------------
	wave_theta_coeff.GetWindowText(theta_coeff);
	double theta_coeff_ = _ttof(theta_coeff);
	//-------------------------------

	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		input_image.Destroy();
	}
	if (waveFFT(&input_image, &output_image, 255, 255, wave_coeff, hor_shift_, ver_shift_, theta_coeff_))
	{
		CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		CStaticHandler::SetBitmap(&input_frame, &input_image_to_show, &input_image);
	}
}


void ImageProcessingDlg::OnBnClickedFourierHighpassInverse()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		if (inverseFourierTransform(&output_image, &input_image))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}


}


void ImageProcessingDlg::OnBnClickedToonShader()
{
	if (!output_image.IsNull())
	{
		output_image.Destroy();
	}
	if (!input_image.IsNull())
	{
		if (toonShading(&output_image, &input_image))
		{
			CStaticHandler::SetBitmap(&output_frame, &output_image_to_show, &output_image);
		}
	}
}
