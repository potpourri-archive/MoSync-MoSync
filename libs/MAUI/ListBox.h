/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** 
* \file ListBox.h 
* \brief List box widget, horizontal or vertical layout
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_LISTBOX_H_
#define _SE_MSAB_MAUI_LISTBOX_H_

#include "Widget.h"
#include "Engine.h"
#include <MAUtil/Environment.h>
#include "TouchMotionTracker.h"

namespace MAUI {
	class ListBox;

	/** \brief Listener interface that receives notifications when an item in a ListBox is selected.
	  **/
	class ItemSelectedListener {
	public:
		/** This function is called whenever the selected item in a listbox changes **/
		virtual void itemSelected(ListBox *sender, Widget *selectedWidget, Widget *unselectedWidget) = 0;
		
		/** This function is called whenever the selection is blocked, due to bounds. 'direction' is -1 when selectPreviousItem was called and 1 when selectedNextItem was called. */
		virtual void blocked(ListBox *sender, int direction) = 0;
	};

	
	/** \brief List box widget for horizontal or vertical lists.
	  *
	  * The MAUI ListBox is a very generic and flexible container that enables laying out widgets in vertical
	  * or horizontal lists. Unlike list boxes you might find in other UI toolkits, this one doesn't only support
	  * text-based items - it supports putting in any type of widget. You could add an image followed by a label,
	  * followed by another, nested ListBox. Furthermore, the navigation is decoupled from input. For instance, it 
	  * provides functions such as selectNextItem() rather than assuming that the way of doing this should always 
	  * be done by pressing the "down" key.  
	   * The widget draws its background by default, use setDrawBackground(false) to disable it.
	  **/
	class ListBox : public Widget, protected MAUtil::TimerListener, protected WidgetListener {
	public:
		enum ListBoxOrientation {
			LBO_HORIZONTAL,
			LBO_VERTICAL
		};

		enum ListBoxAnimationType {
			LBA_NONE,
			LBA_LINEAR
		};

		/** Constructor.
		  * \param orientation controls the orientation of the ListBox
		  * \param animType controls the type of animation used when scrolling the listbox content.
		  * \param wrapping controls the behavior of the listbox when navigating. When set to true (default), the listbox will wrap around to selecting the first element when moving beyond the last one, and equivalently wrap around to the last one when moving back beyond the first one.
		  **/
		ListBox (int x=0, int y=0, int width=0, int height=0,
			ListBoxOrientation orientation=LBO_VERTICAL, 
			ListBoxAnimationType animType=LBA_NONE,
			bool wrapping=true
		);

		/** Destructor **/
		virtual ~ListBox();

		virtual bool isTransparent() const;

		/** Adds the Widget w as a child of the listbox **/
		virtual void add(Widget *w);
		virtual void remove(Widget *w);

		/** Overloaded implementation of Widget::clear(), with the same external semantics.
		    **/
		void clear();

		/** Renders the listbox **/
		//void draw(bool forceDraw=false);

		/** Navigates to the next item in the listbox - down if the orientation is vertical, right if it's horizontal. **/
		void selectNextItem(bool shouldFireListeners=true);
		/** Navigates to the previous item in the listbox - up if the orientation is vertical, left if it's horizontal. **/
		void selectPreviousItem(bool shouldFireListeners=true);
		/** Adds a item selection listener **/
		void addItemSelectedListener(ItemSelectedListener *listener);
		void removeItemSelectedListener(ItemSelectedListener *listener);
		/** Specifies which item in the ListBox should be selected **/
		void setSelectedIndex(int selectedIndex);
		/** Sets the orientation of the ListBox **/
		void setOrientation(ListBoxOrientation orientation);
		/** Sets the type of animation to be used by the ListBox **/
		void setAnimationType(ListBoxAnimationType type);
		/** Controls the wrapping behavior of the ListBox **/
		void setWrapping(bool wrapping=true);
		/** Returns the type of animation used by the List Box **/
		ListBoxAnimationType getAnimationType() const;
		/** Returns whether the ListBox will wrap around when moving beyond the first or last element **/
		bool isWrapping() const;
		/** Returns the index of the ListBox's currently selected child widget **/
		int	getSelectedIndex() const;

		/** Sets autosizing on a ListBox. This autosizes the children of the listbox in the opposite direction of the orientation. I.e. if the list box is vertically oriented, the widgets will be resized to the width of the listbox (the padded width), and vice versa.**/
		void setAutoSize(bool as=true);

		/** Overloaded setWidth. If autosizing is set and the orientation is vertical on the ListBox this will set the width of the children. **/
		//void setWidth(int w);

		/** Overloaded setHeight. If autosizing is set and the orientation is horizontal on the ListBox this will set the height of the children. **/
		//void setHeight(int h);
	
		/** Returns true if child widgets is outside of the front of the list. */
		bool listFrontOutsideBounds() const;

		/** Returns true if child widgets is outside of the back of the list. */
		bool listBackOutsideBounds() const;

		/** Returns the listboxes' current scroll offset in pixels. */
		int getScrollOffset() const;


		virtual bool isFocusable() const;
		virtual bool isFocusableInKeyMode() const;
		void setScrollOffset(int ofs);
		virtual bool pointerPressed(MAPoint2d p, int id);
		virtual bool pointerMoved(MAPoint2d p, int id);
		virtual bool pointerReleased(MAPoint2d p, int id);
		virtual bool keyPressed(int keyCode, int nativeCode);

		void setFocusedWidget(Widget *w);

		virtual void setEnabled(bool enabled=true);
		virtual void setFocused(bool focused=true);

		virtual int getTranslationX() const;
		virtual int getTranslationY() const;

	protected:
		void runTimerEvent();
		void drawWidget();
		void rebuild();
		void boundsChanged(Widget *widget, const Rect& bounds);
		void focusChanged(Widget *widget, bool focused);
		virtual void updateInternal();

		ListenerSet<ItemSelectedListener> mItemSelectedListeners;
		bool mWrapping;
		ListBoxAnimationType mAnimationType;

		ListBoxOrientation mOrientation;
		int mYOffsetFrom;
		int mYOffsetTo;
		int mYOffset;
		int mYOffsetInc;
		int mAnimTimeStart;
		int mSelectedIndex;
		bool mAutoSize;

		bool mTouched;
		TouchMotionTracker mTouchMotionTracker;
		double mTouchDirX, mTouchDirY, mTouchVelX, mTouchVelY;
		double mTimeOfRelease;
		int mTouchedYOffset;

		Widget *mFocusedWidget;
	};
}

#endif /* _SE_MSAB_MAUI_LISTBOX_H_ */
