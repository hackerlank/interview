//-----------------------------------------------------------------------------
// iichantra project
//
// CATEGORY:
// Other
//
// DESCRIPTION:
// Взаимодействие с другими компонентами программы
//-----------------------------------------------------------------------------


#include "iichan_types.h"


void m_log(int stream, const char* message, ...);














#define LOGFILE "iichan.log"
#define IICHAN_LOG(x) m_log(S_LOG, x);