#include "pDockWidget.h"
#include "pDockWidgetTitleBar.h"

#include <QPainter>
#include <QStyle>
#include <QTimer>
#include <QAction>

pDockWidget::pDockWidget( const QString& title, QWidget* parent, Qt::WindowFlags flags )
	: QDockWidget( title, parent, flags )
{
	init();
}

pDockWidget::pDockWidget( QWidget* parent, Qt::WindowFlags flags )
	: QDockWidget( parent, flags )
{
	init();
}

void pDockWidget::init()
{
	mTitleBar = new pDockWidgetTitleBar( this );
	setTitleBarWidget( mTitleBar );
	connect( toggleViewAction(), SIGNAL( triggered( bool ) ), this, SLOT( toggleViewAction_triggered( bool ) ) );
}

void pDockWidget::paintEvent( QPaintEvent* event )
{
	QDockWidget::paintEvent(  event );
	
	if ( isFloating() && style()->objectName().compare( "Oxygen", Qt::CaseInsensitive ) != 0 ) {
		QRect rect = this->rect().adjusted( 0, 0, -1, -1 );
		
		QPainter painter( this );
		painter.setPen( QColor( 145, 142, 142 ) );
		painter.setBrush( Qt::NoBrush );
		painter.drawRect( rect );
	}
}

pDockWidgetTitleBar* pDockWidget::titleBar() const
{
	return mTitleBar;
}

void pDockWidget::toggleViewAction_triggered( bool toggled )
{
	if ( toggled && focusProxy() ) {
		if ( isFloating() ) {
			QTimer::singleShot( 0, this, SLOT( handleWindowActivation() ) );
		}
		else {
			QTimer::singleShot( 0, this, SLOT( handleFocusProxy() ) );
		}
	}
}

void pDockWidget::handleWindowActivation()
{
	activateWindow();
	QTimer::singleShot( 0, this, SLOT( handleFocusProxy() ) );
}

void pDockWidget::handleFocusProxy()
{
	focusProxy()->setFocus();
}
