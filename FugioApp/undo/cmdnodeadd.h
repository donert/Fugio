#ifndef CMDNODEADD_H
#define CMDNODEADD_H

#include <QUndoCommand>
#include <QUuid>

#include <fugio/global_interface.h>
#include <fugio/context_interface.h>
#include <fugio/node_interface.h>

#include "app.h"
#include "contextprivate.h"
#include "contextwidgetprivate.h"
#include <fugio/utils.h>

class CmdNodeAdd : public QUndoCommand
{
public:
	explicit CmdNodeAdd( ContextWidgetPrivate *pContextWidget, const QString &pName, const QUuid &pUuid )
		: mContextWidget( pContextWidget ), mName( pName ), mControlId( pUuid )
	{
		setText( QObject::tr( "Add Node" ) );

		mContext = mContextWidget->context();

		gApp->incrementStatistic( "node-added" );

		gApp->recordData( "node-add", fugio::utils::uuid2string( mControlId ) );
	}

	virtual ~CmdNodeAdd( void )
	{

	}

	virtual void undo( void )
	{
		if( mNode )
		{
			mContext->unregisterNode( mNode->uuid() );
		}
	}

	virtual void redo( void )
	{
		if( mNode.isNull() )
		{
			mContextWidget->saveRecovery();
			
			mNode = mContext->global()->createNode( mName, QUuid::createUuid(), mControlId );

			if( mNode )
			{
				if( mNode->control().isNull() )
				{
					mNode.clear();
				}
			}

			mContextWidget->saveRecovery();
		}

		if( mNode )
		{
			mContext->registerNode( mNode, mControlId );
		}
	}

private:
	ContextWidgetPrivate					*mContextWidget;
	QSharedPointer<fugio::ContextInterface>	 mContext;
	QSharedPointer<fugio::NodeInterface>	 mNode;
	QString									 mName;
	QUuid									 mControlId;
};

#endif // CMDNODEADD_H
