#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_staticText1->SetLabel(_(L"Jasno\u015B\u0107"));
 m_b_threshold->SetLabel(_(L"Pr\u00F3g 128"));
 this->SetBackgroundColour(wxColor(192, 192, 192));
 m_scrolledWindow->SetScrollbars(25, 25, 52, 40);
 m_scrolledWindow->SetBackgroundColour(wxColor(192, 192, 192));
}

void GUIMyFrame1::m_scrolledWindow_update( wxUpdateUIEvent& event )
{
 Repaint();
}

void GUIMyFrame1::m_b_grayscale_click( wxCommandEvent& event )
{
 // TO DO: Konwersja do skali szarosci
	Img_Cpy = Img_Org.ConvertToGreyscale();   
}

void GUIMyFrame1::m_b_blur_click( wxCommandEvent& event )
{
 // TO DO: Rozmywanie obrazu (blur)
	Img_Cpy = Img_Org.Blur(5);
}

void GUIMyFrame1::m_b_mirror_click( wxCommandEvent& event )
{
 // TO DO: Odbicie lustrzane
	Img_Cpy = Img_Org.Mirror();
}

void GUIMyFrame1::m_b_replace_click( wxCommandEvent& event )
{
 // TO DO: Zamiana kolorow 
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.Replace(254, 0, 0, 0, 0, 255);
}

void GUIMyFrame1::m_b_rescale_click( wxCommandEvent& event )
{
 // TO DO: Zmiana rozmiarow do 320x240
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.Rescale(320, 240);
}

void GUIMyFrame1::m_b_rotate_click( wxCommandEvent& event )
{
 // TO DO: Obrot o 30 stopni
	Img_Cpy = Img_Org.Rotate(30 * M_PI / 180.0, wxPoint(Img_Org.GetSize().GetWidth() / 2, Img_Org.GetSize().GetHeight() / 2));
}

void GUIMyFrame1::m_b_rotate_hue_click( wxCommandEvent& event )
{
 // TO DO: Przesuniecie Hue o 180 stopni
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.RotateHue(180.0 / 360.0);
}

void GUIMyFrame1::m_b_mask_click( wxCommandEvent& event )
{
 // TO DO: Ustawienie maski obrazu
	Img_Cpy = Img_Org.Copy();
	Img_Cpy.SetMaskFromImage(Img_Mask, 0, 0, 0);
}

void GUIMyFrame1::m_s_brightness_scroll( wxScrollEvent& event )
{
// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
// Brightness(...), ktora zmienia jasnosc. W tym miejscu nic nie
// zmieniamy. Do uzupelnienia pozostaje funkcja Brightness(...)
Brightness(m_s_brightness->GetValue() - 100);
Repaint();
}

void GUIMyFrame1::m_s_contrast_scroll( wxScrollEvent& event )
{
// Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
// Contrast(...), ktora zmienia kontrast. W tym miejscu nic nie
// zmieniamy. Do uzupelnienia pozostaje funkcja Contrast(...)
Contrast(m_s_contrast->GetValue() - 100);
Repaint();
}

int GUIMyFrame1::verticalGradient(const wxImage& image, int x, int y) {
	int gradient = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			gradient += image.GetRed(x + i, y + j) * i;
		}
	}
	return gradient;
}

void GUIMyFrame1::m_b_prewitt_click( wxCommandEvent& event )
{
 // TO DO: Pionowa maska Prewitta
	Img_Cpy = Img_Org.Copy();
	unsigned char* data = Img_Cpy.GetData();
	size_t width = Img_Cpy.GetWidth();
	size_t height = Img_Cpy.GetHeight();
	for (int y = 1; y < height - 1; y++){
		for (int x = 1; x < width - 1; x++){
			int gradient = verticalGradient(Img_Org, x, y);
			if (gradient < 0)
				gradient = 0;
			if (gradient > 255)
				gradient = 255;
			for (int c = 0; c < 3; c++)
				data[(y * width + x) * 3 + c] = gradient;
		}
	}
}

void GUIMyFrame1::m_b_threshold_click( wxCommandEvent& event )
{
 // TO DO: Prog o wartosci 128 dla kazdego kanalu niezaleznie
	Img_Cpy = Img_Org.Copy();
	unsigned char* data = Img_Cpy.GetData();
	size_t size = 3 * Img_Cpy.GetWidth() * Img_Cpy.GetHeight();
	for (size_t i = 0; i < size; i++){
		if (data[i] > 128)
			data[i] = 255;
		else
			data[i] = 0;
	}
}


void GUIMyFrame1::Contrast(int value)
{
 // TO DO: Zmiana kontrastu obrazu. value moze przyjmowac wartosci od -100 do 100
	Img_Cpy = Img_Org.Copy();
	unsigned char* data = Img_Cpy.GetData();
	size_t size = 3 * Img_Cpy.GetWidth() * Img_Cpy.GetHeight();
	double contrast = pow((100.0 + value) / 100.0, 5.0);
	for (size_t i = 0; i < size; i++) {
		int pixel = data[i];
		pixel = static_cast<int>((pixel - 128) * contrast + 128);
		if (pixel < 0)
			pixel = 0;
		if (pixel > 255)
			pixel = 255;
		data[i] = static_cast<unsigned char>(pixel);
	}
}

void GUIMyFrame1::Brightness(int value)
{
 // TO DO: Zmiana jasnosci obrazu. value moze przyjmowac wartosci od -100 do 100
	Img_Cpy = Img_Org.Copy();
	unsigned char* data = Img_Cpy.GetData();
	size_t size = 3 * Img_Cpy.GetWidth() * Img_Cpy.GetHeight();
	for (size_t i = 0; i < size; i++){
		if (data[i] + value < 0)
			data[i] = 0;
		else if (data[i] + value > 255)
			data[i] = 255;
		else
			data[i] += value;
	}
}

void GUIMyFrame1::Repaint()
{
	wxBitmap bitmap(Img_Cpy);          // Tworzymy tymczasowa bitmape na podstawie Img_Cpy
	wxClientDC dc(m_scrolledWindow);   // Pobieramy kontekst okna
	m_scrolledWindow->DoPrepareDC(dc); // Musimy wywolac w przypadku wxScrolledWindow, zeby suwaki prawidlowo dzialaly
	dc.DrawBitmap(bitmap, 0, 0, true); // Rysujemy bitmape na kontekscie urzadzenia
}