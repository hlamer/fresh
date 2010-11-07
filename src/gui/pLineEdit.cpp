#include "pLineEdit.h"
#include "pIconManager.h"

#include <QToolButton>
#include <QStyleOptionFrameV3>
#include <QPainter>
#include <QTimer>
#include <QEvent>
#include <QDebug>

const int pLineEdit::mTimeOut = 1000;

pLineEdit::pLineEdit( QWidget* parent )
	: QLineEdit( parent )
{
	init();
}

pLineEdit::pLineEdit( const QString& contents, QWidget* parent )
	: QLineEdit( contents, parent )
{
	init();
}

QMenu* pLineEdit::menu() const
{
	return tbSearch->menu();
}

void pLineEdit::setMenu( QMenu* menu )
{
	tbSearch->setMenu( menu );
}

bool pLineEdit::isSearchButtonVisible() const
{
	return tbSearch->isVisible();
}

bool pLineEdit::isPromptVisible() const
{
	return mPromptVisible;
}

QString pLineEdit::promptText() const
{
	return mPromptText;
}

void pLineEdit::setSearchButtonVisible( bool visible )
{
	tbSearch->setVisible( visible );
	
	int left;
	int top;
	int right;
	int bottom;
	
	getTextMargins( &left, &top, &right, &bottom );
	
	if ( visible ) {
		left = mMargin +mSpacing;
	}
	else {
		left = 0;
	}
	
	setTextMargins( left, top, right, bottom );
}

void pLineEdit::setPromptVisible( bool visible )
{
	mPromptVisible = visible;
	update();
}

void pLineEdit::setPromptText( const QString& prompt )
{
	mPromptText = prompt;
	update();
}

void pLineEdit::clickSearchButton()
{
	tbSearch->click();
}

void pLineEdit::clickClearButton()
{
	tbClear->click();
}

void pLineEdit::paintEvent( QPaintEvent* event )
{
	QLineEdit::paintEvent( event );
	
	if ( mPromptVisible && !mPromptText.isEmpty() && text().isEmpty() ) {
		QStyleOptionFrameV3 option;
		initStyleOption( &option );
		
		int left;
		int top;
		int right;
		int bottom;
		
		getTextMargins( &left, &top, &right, &bottom );
		
		const Qt::Alignment va = style()->visualAlignment( layoutDirection(), alignment() );
		const QRect rect = style()->subElementRect( QStyle::SE_LineEditContents, &option, this ).adjusted( 2, 0, 0, 0 ).adjusted( left, top, -right, -bottom );
		const QFontMetrics fm( font() );
		const QString text = fm.elidedText( mPromptText, Qt::ElideRight, rect.width() );
		QPainter painter( this );
		
		painter.setPen( palette().color( QPalette::Disabled, QPalette::Text ) );
		painter.drawText( rect, va, text );
	}
}

void pLineEdit::resizeEvent( QResizeEvent* event )
{
	QLineEdit::resizeEvent( event );
	
	tbSearch->resize( QSize( mMargin, height() -2 ) );
	tbSearch->move( 3, 1 );
	tbClear->resize( QSize( mMargin, height() -2 ) );
	tbClear->move( width() -mMargin -3, 0 );
}

void pLineEdit::focusInEvent( QFocusEvent* event )
{
	QLineEdit::focusInEvent( event );

	if ( mPromptVisible ) {
		setPromptVisible( false );
	}
}

void pLineEdit::focusOutEvent( QFocusEvent* event )
{
	QLineEdit::focusOutEvent( event );

	if ( text().isEmpty() ) {
		setPromptVisible( true );
	}
}

void pLineEdit::changeEvent( QEvent* event )
{
	QLineEdit::changeEvent( event );
	
	if ( text().isEmpty() && event->type() == QEvent::EnabledChange ) {
		if ( !isEnabled() ) {
			setPromptVisible( true );
		}
	}
}

void pLineEdit::init()
{
	mMargin = sizeHint().height() -2;
	mSpacing = 0;
	
	tbSearch = new QToolButton( this );
	tbSearch->setIcon( pIconManager::icon( "search.png", ":/fresh" ) );
	tbSearch->setToolTip( tr( "Search Options" ) );
	tbSearch->setStyleSheet( "QToolButton { border: none; padding: 0px; } QToolButton::menu-indicator { right: -2px; bottom: -2px; }" );
	tbSearch->setCursor( Qt::ArrowCursor );
	tbSearch->setFocusPolicy( Qt::NoFocus );
	tbSearch->setPopupMode( QToolButton::InstantPopup );
	
	tbClear = new QToolButton( this );
	tbClear->setIcon( pIconManager::icon( "edit-clear-rtl.png", ":/fresh" ) );
	tbClear->setToolTip( tr( "Clear" ) );
	tbClear->setStyleSheet( "QToolButton { border: none; padding: 0px; }" );
	tbClear->setCursor( Qt::ArrowCursor );
	tbClear->setFocusPolicy( Qt::NoFocus );
	
	setSearchButtonVisible( true );
	setClearButtonVisible( false );
	setPromptText( "Search..." );
	setPromptVisible( true );
	
	mTimer = new QTimer( this );
	mTimer->setInterval( mTimeOut );
	mTimer->setSingleShot( true );
	
	connect( this, SIGNAL( textChanged( const QString& ) ), this, SLOT( _q_textChanged( const QString& ) ) );
	connect( tbSearch, SIGNAL( clicked() ), this, SIGNAL( searchButtonClicked() ) );
	connect( tbSearch, SIGNAL( triggered( QAction* ) ), this, SIGNAL( searchButtonActionTriggered( QAction* ) ) );
	connect( tbClear, SIGNAL( clicked() ), this, SLOT( clear() ) );
	connect( tbClear, SIGNAL( clicked() ), this, SIGNAL( clearButtonClicked() ) );
	connect( mTimer, SIGNAL( timeout() ), this, SLOT( timer_timeout() ) );
}

void pLineEdit::setClearButtonVisible( bool visible )
{
	tbClear->setVisible( visible );
	
	int left;
	int top;
	int right;
	int bottom;
	
	getTextMargins( &left, &top, &right, &bottom );
	
	if ( visible ) {
		right = mMargin +mSpacing;
	}
	else {
		right = 0;
	}
	
	setTextMargins( left, top, right, bottom );
}

void pLineEdit::_q_textChanged( const QString& text )
{
	setClearButtonVisible( !text.isEmpty() );
	mTimer->start();
}

void pLineEdit::timer_timeout()
{
	emit searchTextChanged( text() );
}