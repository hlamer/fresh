/*!
	\file pToolButton.h
	\date 2008-01-14T00:27:54
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A corner button used by the pTabbedWorkspaceCorner class
*/

class pToolButton : QToolButton
{
%TypeHeaderCode
#include <gui/pToolButton.h>
%End

public:
	pToolButton( QWidget* parent, QBoxLayout::Direction direction = QBoxLayout::LeftToRight );

	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;

	QBoxLayout::Direction direction() const;
	QVariant userData() const;

public slots:
	void setDirection( QBoxLayout::Direction direction );
	void setUserData( const QVariant& data );

protected:
	enum CursorArea {
		caNone = 0,
		caArrow,
		caButton,
		caArrowClicked,
		caButtonClicked
	};
	
	virtual void paintEvent( QPaintEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseMoveEvent( QMouseEvent* event );
	virtual void mouseReleaseEvent( QMouseEvent* event );

	void paint( QStyleOptionToolButton* option );
	QSize internalSize( Qt::Orientation orientation ) const;
	pToolButton::CursorArea cursorArea( const QPoint& pos = QPoint() ) const;

	QMenu* hasMenu() const;
	bool menuButtonDown() const;
};
