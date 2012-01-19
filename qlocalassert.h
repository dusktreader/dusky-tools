#pragma once

#include <QString>
#include "localassert.h"

#define ASSERT_QMSG( expr, msg ) ASSERT_MSG( expr, msg.toStdString() )
