#include <Windows.h>
#include <ntsecapi.h>
#include <stdio.h>
#include <iostream>
#include <wtsapi32.h>

#pragma once
#ifndef SECURITY_WIN32
#define SECURITY_WIN32
#endif
#include "sspi.h"

// Прототип функции callback
void NTAPI LogonCallback(PSE_ADT_PARAMETER_ARRAY AuditParameters);

// Главная функция
int main() {

    HANDLE hEvent;
    NTSTATUS ntsResult;
    DWORD dwResult;

    // Create an event object.
    hEvent = CreateEvent(
        NULL,  // child processes cannot inherit 
        FALSE, // automatically reset event
        FALSE, // start as a nonsignaled event
        NULL   // do not need a name
    );

    // Check that the event was created.
    if (hEvent == NULL)
    {
        wprintf(L"Event object creation failed: %d\n", GetLastError());
        return 1;
    }
    // Register to receive auditing policy change notifications.
    ntsResult = LsaRegisterPolicyChangeNotification(
        PolicyNotifyAuditEventsInformation,
        //PolicyNotifyMax,
        hEvent
    );
    if (0 != ntsResult)
    {
        wprintf(L"LsaRegisterPolicyChangeNotification failed.\n");
        CloseHandle(hEvent);
        return 1;
    }

    // Wait for the event to be triggered.
    dwResult = WaitForSingleObject(
        hEvent, // handle to the event object
        300000  // time-out interval, in milliseconds
    );

    // The wait function returned.
    if (dwResult == WAIT_OBJECT_0)
    {  // received the notification signal
        wprintf(L"Notification received.\n");
    }
    else
    {  // received a time-out or error
        wprintf(L"Notification was not received.\n");
    }
    // Unregister for notification.
    LsaUnregisterPolicyChangeNotification(
        PolicyNotifyAuditEventsInformation,
        hEvent
    );

    // Free the event handle.
    CloseHandle(hEvent);

    return 0;
}
