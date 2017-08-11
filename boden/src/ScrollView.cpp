#include <bdn/init.h>
#include <bdn/ScrollView.h>

#include <bdn/LayoutCoordinator.h>
#include <bdn/IScrollViewCore.h>

namespace bdn
{

ScrollView::ScrollView()
    : _verticalScrollingEnabled(true)
{
	initProperty<bool, IScrollViewCore, &IScrollViewCore::setHorizontalScrollingEnabled, (int)PropertyInfluence_::preferredSize | (int)PropertyInfluence_::childLayout>(_horizontalScrollingEnabled);
    initProperty<bool, IScrollViewCore, &IScrollViewCore::setVerticalScrollingEnabled, (int)PropertyInfluence_::preferredSize | (int)PropertyInfluence_::childLayout>(_verticalScrollingEnabled);
}




}

