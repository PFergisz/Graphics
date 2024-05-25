#include "GUIMyFrame1.h"
#include <vector>
#include <fstream>
#include "vecmat.h"

struct Point {
 float x, y, z;
 Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
 int R, G, B;
 Color(int _R, int _G, int _B) : R(_R), G(_G), B(_B) {}
};

struct Segment {
 Point begin, end;
 Color color;
 Segment(Point _begin, Point _end, Color _color) : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1( wxWindow* parent )
:
MyFrame1( parent )
{
 m_button_load_geometry->SetLabel(_("Wczytaj Geometri\u0119"));
 m_staticText25->SetLabel(_("Obr\u00F3t X:"));
 m_staticText27->SetLabel(_("Obr\u00F3t Y:"));
 m_staticText29->SetLabel(_("Obr\u00F3t Z:"));

 WxSB_TranslationX->SetRange(0, 200); WxSB_TranslationX->SetValue(100);
 WxSB_TranslationY->SetRange(0, 200); WxSB_TranslationY->SetValue(100);
 WxSB_TranslationZ->SetRange(0, 200); WxSB_TranslationZ->SetValue(100);

 WxSB_RotateX->SetRange(0, 360); WxSB_RotateX->SetValue(0);
 WxSB_RotateY->SetRange(0, 360); WxSB_RotateY->SetValue(0);
 WxSB_RotateZ->SetRange(0, 360); WxSB_RotateZ->SetValue(0);

 WxSB_ScaleX->SetRange(1, 200); WxSB_ScaleX->SetValue(100);
 WxSB_ScaleY->SetRange(1, 200); WxSB_ScaleY->SetValue(100);
 WxSB_ScaleZ->SetRange(1, 200); WxSB_ScaleZ->SetValue(100);
}

void GUIMyFrame1::WxPanel_Repaint( wxUpdateUIEvent& event )
{
 Repaint();
}

void GUIMyFrame1::m_button_load_geometry_click( wxCommandEvent& event )
{
 wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

 if (WxOpenFileDialog.ShowModal() == wxID_OK)
 {
  double x1, y1, z1, x2, y2, z2;
  int r, g, b;

  std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
  if (in.is_open())
  {
   data.clear();
   while (!in.eof())
   {
    in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
    data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
   }
   in.close();
  }
 }
}

void GUIMyFrame1::Scrolls_Updated( wxScrollEvent& event )
{
WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));


Repaint();
}

void GUIMyFrame1::Repaint() {
    Matrix4 scale_matrix;
    scale_matrix.SetScale(WxSB_ScaleX->GetValue() / 100.0,
        WxSB_ScaleY->GetValue() / 100.0,
        WxSB_ScaleZ->GetValue() / 100.0);


    Matrix4 rotateX_matrix;
    double alpha = WxSB_RotateX->GetValue() * M_PI / 180.0;
    rotateX_matrix.SetRotationX(alpha);

    Matrix4 rotateY_matrix;
    alpha = WxSB_RotateY->GetValue() * M_PI / 180.0;
    rotateY_matrix.SetRotationY(alpha);

    Matrix4 rotateZ_matrix;
    alpha = WxSB_RotateZ->GetValue() * M_PI / 180.0;
    rotateZ_matrix.SetRotationZ(alpha);

    Matrix4 translation_matrix;
    translation_matrix.SetTranslation((WxSB_TranslationX->GetValue() - 100.0) / 50.0,
        (WxSB_TranslationY->GetValue() - 100.0) / 50.0,
        (WxSB_TranslationZ->GetValue() - 100.0) / 50.0);

    Matrix4 zhelper_matrix;
    zhelper_matrix.SetIdentity();

    Matrix4 windowhelper_matrix;
    wxSize panelSize = WxPanel->GetSize();
    windowhelper_matrix.SetWindowCoordinates(panelSize.GetWidth() / 2.0,
        -panelSize.GetHeight() / 2.0,
        panelSize.GetWidth() / 2.0,
        panelSize.GetHeight() / 2.0);

    Matrix4 transform1_matrix = translation_matrix * rotateX_matrix * rotateY_matrix * rotateZ_matrix * scale_matrix;
    Matrix4 transform2_matrix = windowhelper_matrix * zhelper_matrix;

    wxClientDC _dc(WxPanel);
    wxBufferedDC dc(&_dc);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();



    for (Segment segment : data) {
        dc.SetPen(wxPen(wxColour(segment.color.R, segment.color.G, segment.color.B)));

        Vector4 start;
        start.Set(segment.begin.x, segment.begin.y, segment.begin.z);
        start = transform1_matrix * start;

        Vector4 end;
        end.Set(segment.end.x, segment.end.y, segment.end.z);
        end = transform1_matrix * end;

        start.Homogenize();
        end.Homogenize();

        auto clip = 1.0f;

        if ((start.GetZ() > -clip && end.GetZ() <= -clip) || (end.GetZ() > -clip && start.GetZ() <= -clip)) {
            Vector4 a, b;
            if (end.GetZ() <= -clip) {
                a = end;
                b = start;
            }
            else {
                a = start;
                b = end;
            }
            double tmp = abs((-clip - a.data[2]) / (b.data[2] - a.data[2]));
            a.Interpolate(b, tmp, -clip);

            start = transform2_matrix * a;
            end = transform2_matrix * b;
            start.Homogenize();
            end.Homogenize();
        }
        else if (start.GetZ() <= -clip && end.GetZ() <= -clip) {
            continue;
        }
        else {
            start = transform2_matrix * start;
            end = transform2_matrix * end;
            start.Homogenize();
            end.Homogenize();
        }

        dc.DrawLine(start.GetX(), start.GetY(), end.GetX(), end.GetY());
    }
}


