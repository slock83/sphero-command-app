/*************************************************************************
	ActionHandler  - Generic template to handle diverse actions
                             -------------------
	started                : 08/05/2015
*************************************************************************/

//--------------------------------------------------------- Local includes
//
//------------------------------------------------ Constructors/Destructor

/**
 * @brief ActionHandler : Constructor
 */
template<typename ...T>
ActionHandler<T...>::ActionHandler():_listenerList()
{}


template<typename ...T>
ActionHandler<T...>::~ActionHandler ( )
{}


//--------------------------------------------------------- Public methods

/**
 * @brief reportAction : Reports the ocurred actions to actionHandler.
 *				All listeners are then notified.
 * @param action : A pointer to the action informations data structure
 */
template<typename ...T>
void ActionHandler<T...>::reportAction(T... action)
{
	typename listenerList_t::iterator it;
	for(it = _listenerList.begin() ; it != _listenerList.end() ; it++)
	{
		(*it)(action...);
	}
}


/**
 * @brief addActionListener : Registers a new listener
 * @param : listener : The new listener which will be registered
 */
template<typename ...T>
void ActionHandler<T...>::addActionListener(listener_t listener)
{
	_listenerList.push_front(listener);
}


/**
 * @brief clearListener : Unregisters all listeners
 */
template<typename ...T>
void ActionHandler<T...>::clearListener()
{
	listenerList_t empty;
	std::swap(_listenerList, empty);

}
