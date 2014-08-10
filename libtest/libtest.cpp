// libtest.cpp
#include "pch.h"
#include "libtest.h"
#include "testlibevent.h"
using namespace libtest;
using namespace Platform;

WindowsPhoneRuntimeComponent::WindowsPhoneRuntimeComponent()
{
}

void WindowsPhoneRuntimeComponent::startService()
{
	::start_service(5555);
}
