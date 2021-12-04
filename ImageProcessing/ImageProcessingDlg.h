
// ImageProcessingDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ImageHandler.h"
#include "afxcolorbutton.h"

// ImageProcessingDlg dialog
class ImageProcessingDlg : public CDialogEx
{
	// Construction
public:
	ImageProcessingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImageProcessing_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLoad();
protected:
	CStatic input_frame;
	CImage input_image;
	CImage output_image;
	CImage output_image_to_show;
	CImage input_image_to_show;
	CStatic output_frame;
	int width;
	int height;
	int bpp;
public:
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedResize();
	CEdit resize_number;
	afx_msg void OnBnClickedTranslation();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRotate();
	CEdit rotate_number;
	CEdit translation_x;
	CEdit translation_number_x;
	CEdit translation_number_y;
	afx_msg void OnBnClickedOutputAsInput();
	afx_msg void OnBnClickedGray();
	afx_msg void OnBnClickedGrayHist();
	afx_msg void OnBnClickedSmooth();
	afx_msg void OnBnClickedEdgeDetection();
	CComboBox smooth_algorithm;
	CComboBox edge_detect_algorithm;
	afx_msg void OnBnClickedHalfSize();
	afx_msg void OnBnClickedFouriertransform();
	afx_msg void OnBnClickedDoubleSize();
	CEdit gamma;
	afx_msg void OnBnClickedGamma();
	afx_msg void OnBnClickedWave();
	CEdit wave_coeff;
	CEdit wave_theta_coeff;
	CEdit wave_hor_shift;
	CEdit wave_ver_shift;
	afx_msg void OnBnClickedSubcolor();
	CEdit subColorRange;
	CMFCColorButton subColorType;
	afx_msg void OnBnClickedDirectwaveFft();
	afx_msg void OnBnClickedFourierHighpassInverse();
	afx_msg void OnBnClickedToonShader();
};
