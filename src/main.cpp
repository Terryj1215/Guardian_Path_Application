#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application metadata
    QApplication::setApplicationName("Guardian Path");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("GuardianPath");
    
    // Apply global stylesheet
    app.setStyleSheet(R"(
        /* Guardian Path - Security-First Dark Theme */
        QWidget {
            background-color: #1a1f2e;
            color: #f0f0f5;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 14px;
        }
        
        QMainWindow {
            background-color: #141824;
        }
        
        /* Cards */
        QFrame[class="card"] {
            background-color: #232a3d;
            border: 1px solid #2d3548;
            border-radius: 12px;
            padding: 16px;
        }
        
        /* Primary Button */
        QPushButton[class="primary"] {
            background-color: #4a6cf7;
            color: #ffffff;
            border: none;
            border-radius: 8px;
            padding: 12px 24px;
            font-weight: 600;
            font-size: 14px;
        }
        
        QPushButton[class="primary"]:hover {
            background-color: #5b7af8;
        }
        
        QPushButton[class="primary"]:pressed {
            background-color: #3d5ce6;
        }
        
        /* Secondary Button */
        QPushButton[class="secondary"] {
            background-color: #2d3548;
            color: #f0f0f5;
            border: 1px solid #3d4560;
            border-radius: 8px;
            padding: 12px 24px;
            font-weight: 500;
        }
        
        QPushButton[class="secondary"]:hover {
            background-color: #3d4560;
        }
        
        /* SOS Button */
        QPushButton[class="sos"] {
            background-color: #dc2626;
            color: #ffffff;
            border: none;
            border-radius: 50%;
            font-weight: 700;
            font-size: 18px;
        }
        
        QPushButton[class="sos"]:hover {
            background-color: #ef4444;
        }
        
        QPushButton[class="sos"]:pressed {
            background-color: #b91c1c;
        }
        
        /* Safe/Success Status */
        QLabel[class="safe"] {
            background-color: #22c55e;
            color: #141824;
            border-radius: 4px;
            padding: 4px 8px;
            font-weight: 600;
        }
        
        /* Warning Status */
        QLabel[class="warning"] {
            background-color: #eab308;
            color: #141824;
            border-radius: 4px;
            padding: 4px 8px;
            font-weight: 600;
        }
        
        /* Sidebar */
        QFrame[class="sidebar"] {
            background-color: #111520;
            border-right: 1px solid #2d3548;
        }
        
        QPushButton[class="nav-item"] {
            background-color: transparent;
            color: #a0a5b5;
            border: none;
            border-radius: 8px;
            padding: 12px 16px;
            text-align: left;
            font-size: 14px;
        }
        
        QPushButton[class="nav-item"]:hover {
            background-color: #232a3d;
            color: #f0f0f5;
        }
        
        QPushButton[class="nav-item"][active="true"] {
            background-color: #4a6cf7;
            color: #ffffff;
        }
        
        /* Input Fields */
        QLineEdit, QTextEdit {
            background-color: #232a3d;
            color: #f0f0f5;
            border: 1px solid #3d4560;
            border-radius: 8px;
            padding: 10px 14px;
        }
        
        QLineEdit:focus, QTextEdit:focus {
            border-color: #4a6cf7;
        }
        
        /* Slider */
        QSlider::groove:horizontal {
            background-color: #2d3548;
            height: 8px;
            border-radius: 4px;
        }
        
        QSlider::handle:horizontal {
            background-color: #4a6cf7;
            width: 20px;
            height: 20px;
            margin: -6px 0;
            border-radius: 10px;
        }
        
        QSlider::sub-page:horizontal {
            background-color: #4a6cf7;
            border-radius: 4px;
        }
        
        /* Toggle Switch (using QCheckBox) */
        QCheckBox::indicator {
            width: 44px;
            height: 24px;
            border-radius: 12px;
            background-color: #3d4560;
        }
        
        QCheckBox::indicator:checked {
            background-color: #22c55e;
        }
        
        /* Scroll Area */
        QScrollArea {
            border: none;
            background-color: transparent;
        }
        
        QScrollBar:vertical {
            background-color: #1a1f2e;
            width: 8px;
            border-radius: 4px;
        }
        
        QScrollBar::handle:vertical {
            background-color: #3d4560;
            border-radius: 4px;
            min-height: 30px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: #4a6cf7;
        }
        
        /* Labels */
        QLabel[class="title"] {
            font-size: 24px;
            font-weight: 700;
            color: #f0f0f5;
        }
        
        QLabel[class="subtitle"] {
            font-size: 16px;
            font-weight: 500;
            color: #a0a5b5;
        }
        
        QLabel[class="muted"] {
            color: #6b7280;
            font-size: 13px;
        }
    )");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
