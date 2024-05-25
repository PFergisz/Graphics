#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
	this->SetBackgroundColour(wxColor(192, 192, 192));
	m_bitmap.Create(PANEL_WIDTH, PANEL_HEIGHT, 24);
	m_image.Create(PANEL_WIDTH, PANEL_HEIGHT);
	PrepareData(0);
	Draw(NoPoints, FunctionData, ShowColor, alpha);
	Repaint();
}

void GUIMyFrame1::m_button1_click( wxCommandEvent& event )
{
// TODO: Implement m_button1_click
	PrepareData(1);
	choice = 1;
	Draw(NoPoints, FunctionData, ShowColor, alpha);
	Repaint();
}

void GUIMyFrame1::m_button2_click( wxCommandEvent& event )
{
// TODO: Implement m_button2_click
	PrepareData(2);
	choice = 2;
	Draw(NoPoints, FunctionData, ShowColor, alpha);
	Repaint();
}

void GUIMyFrame1::m_button3_click( wxCommandEvent& event )
{
// TODO: Implement m_button3_click
	PrepareData(3);
	choice = 3;
	Draw(NoPoints, FunctionData, ShowColor, alpha);
	Repaint();
}

void GUIMyFrame1::m_button4_click( wxCommandEvent& event )
{
// TODO: Implement m_button4_click
	PrepareData(4);
	choice = 4;
	Draw(NoPoints, FunctionData, ShowColor, alpha);
	Repaint();
}

void GUIMyFrame1::m_cb_color_click( wxCommandEvent& event )
{
// TODO: Implement m_cb_color_click
	ShowColor == true ? ShowColor = false : ShowColor = true;
	Draw(NoPoints, FunctionData, ShowColor, alpha);
	Repaint();
}

void GUIMyFrame1::m_s_rotation_onscroll( wxScrollEvent& event )
{
	m_st_rotation->SetLabel(wxString::Format("Obrot: %d stopni.",m_s_rotation->GetValue()));
	alpha = m_s_rotation->GetValue() * M_PI / 180;
	Draw(NoPoints, FunctionData, ShowColor, alpha);
	Repaint();
}

void GUIMyFrame1::m_s_tilt_onscroll( wxScrollEvent& event )
{
// TODO: Implement m_s_tilt_onscroll
	tilt = m_s_tilt->GetValue();
	Draw(NoPoints, FunctionData, ShowColor, alpha);
	Repaint();

}


void GUIMyFrame1::Repaint()
{
	wxMemoryDC memDC;
	memDC.SelectObject(m_bitmap);
	wxClientDC dc(m_panel1);
	dc.Blit(-20, 0, 500, 500, &memDC, 0, 0);
	memDC.SelectObject(wxNullBitmap);
}



void GUIMyFrame1::Draw(int N, float d[100][3], bool ShowColor, double alpha)
{
	wxMemoryDC memDC;
	memDC.SelectObject(m_bitmap);
	memDC.SetBackground(*wxWHITE_BRUSH);
	memDC.Clear();
	memDC.SetPen(*wxBLACK_PEN);
	memDC.SetBrush(*wxTRANSPARENT_BRUSH);

	float width = 1;
	float height = 2 + tilt * 0.05;

	PrepareData(choice);

	float coso = cos(alpha);
	float sino = sin(alpha);

	for (int i = 0; i < N; i++)
	{
		float x = d[i][0];
		float y = d[i][1];

		d[i][0] = x * coso - y * sino;
		d[i][1] = x * sino + y * coso;
	}


	auto values = new float[500][500];
	constexpr float epsilon = 0.0001f;
	#pragma omp parallel for
		for (int y = 0; y < 500; y++) {
			for (int x = 0; x < 500; x++) {
				float weightedSum = 0.0f;
				float totalWeight = 0.0f;


				for (int i = 0; i < N; i++) {
					float distSquared = (x * 1.25 / 100.0 - 2.5 - d[i][0]) * (x * 1.25 / 100.0 - 2.5 - d[i][0]) +
						(-y * 1.25 / 100.0 + 2.5 - d[i][1]) * (-y * 1.25 / 100.0 + 2.5 - d[i][1]);
					float weight = 1.0f / (distSquared + epsilon);
					weightedSum += weight * d[i][2];
					totalWeight += weight;
				}

				values[x][y] = totalWeight > 0 ? weightedSum / totalWeight : 0.0f;
			}
		}
	


	

	float min = d[0][2], max = d[0][2];
	for (int i = 0; i < N; i++) {
		if (d[i][2] < min)
			min = d[i][2];
		if (d[i][2] > max)
			max = d[i][2];
	}

	if (ShowColor==true)
	{
		unsigned char* data = new unsigned char[750000];
		for (int x = 0; x < 500; x++) {
			for (int y = 0; y < 500; y++) {
				float color = ((values[x][y]) - min) / (max - min);
				data[y * 500 * 3 + x * 3 + 0] = 255 * color;
				data[y * 500 * 3 + x * 3 + 1] = 0;
				data[y * 500 * 3 + x * 3 + 2] = 255 - 255 * color;
			}
		}

		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 80; j++) {
				float color = ((values[i * 10][j * 5] - min)) / (max - min) * 50 + 51;
				memDC.SetPen(wxColor(data[j * 5 * 500 * 3 + i * 10 * 3 + 0], data[j * 5 * 500 * 3 + i * 10 * 3 + 1], data[j * 5 * 500 * 3 + i * 10 * 3 + 2]));

				memDC.SetBrush(wxColor(data[j * 5 * 500 * 3 + i * 10 * 3 + 0], data[j * 5 * 500 * 3 + i * 10 * 3 + 1], data[j * 5 * 500 * 3 + i * 10 * 3 + 2]));
				memDC.DrawRectangle(500 - (width * i + width * j * 5), 150 + height * i - color, width * 10, color);
			}
		}
		return;
	}
	if(ShowColor==false)
	{
		unsigned char* data = new unsigned char[750000];
		unsigned char* new_data = new unsigned char[750000];
		for (int x = 0; x < 500; x++) {
			for (int y = 0; y < 500; y++) {
				float color = (values[x][y] - min) / (max - min);
				data[y * 500 * 3 + x * 3 + 0] = 255 * color;
				data[y * 500 * 3 + x * 3 + 1] = 255 * color;
				data[y * 500 * 3 + x * 3 + 2] = 255 * color;
			}

		}

		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 80; j++) {
				float color = ((values[i * 10][j * 5] - min)) / (max - min) * 50 + 51;
				memDC.SetPen(wxColor(data[j * 5 * 500 * 3 + i * 10 * 3 + 0], data[j * 5 * 500 * 3 + i * 10 * 3 + 1], data[j * 5 * 500 * 3 + i * 10 * 3 + 2]));
				memDC.SetBrush(wxColor(data[j * 5 * 500 * 3 + i * 10 * 3 + 0], data[j * 5 * 500 * 3 + i * 10 * 3 + 1], data[j * 5 * 500 * 3 + i * 10 * 3 + 2]));
				memDC.DrawRectangle(500 - (width * i + width * j * 5), 150 + height * i - color, width * 10, color);
			}
		}
	}

}


