#include "MainSurface.h"

using namespace Ui;

MainSurface::MainSurface(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainSurfaceClass())
{
	ui->setupUi(this);

	sw_ui = new StackWidget();

	connect(ui->start, &QPushButton::clicked, this, &MainSurface::openStackWidget);
}

MainSurface::~MainSurface()
{
	delete ui;
}

void MainSurface::openStackWidget() {
	this->close();
	sw_ui->initialization();
	sw_ui->show();
}
