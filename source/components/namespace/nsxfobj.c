
/******************************************************************************
 * 
 * Module Name: nsapiobj - Public interfaces to the ACPI subsystem
 *                         ACPI Object oriented interfaces
 *
 *****************************************************************************/

/******************************************************************************
 *
 * 1. Copyright Notice
 *
 * Some or all of this work - Copyright (c) 1999, Intel Corp.  All rights
 * reserved.
 *
 * 2. License
 *
 * 2.1. This is your license from Intel Corp. under its intellectual property
 * rights.  You may have additional license terms from the party that provided
 * you this software, covering your right to use that party's intellectual
 * property rights.
 *
 * 2.2. Intel grants, free of charge, to any person ("Licensee") obtaining a
 * copy of the source code appearing in this file ("Covered Code") an
 * irrevocable, perpetual, worldwide license under Intel's copyrights in the
 * base code distributed originally by Intel ("Original Intel Code") to copy,
 * make derivatives, distribute, use and display any portion of the Covered
 * Code in any form, with the right to sublicense such rights; and
 *
 * 2.3. Intel grants Licensee a non-exclusive and non-transferable patent
 * license (with the right to sublicense), under only those claims of Intel
 * patents that are infringed by the Original Intel Code, to make, use, sell,
 * offer to sell, and import the Covered Code and derivative works thereof
 * solely to the minimum extent necessary to exercise the above copyright
 * license, and in no event shall the patent license extend to any additions
 * to or modifications of the Original Intel Code.  No other license or right
 * is granted directly or by implication, estoppel or otherwise;
 *
 * The above copyright and patent license is granted only if the following
 * conditions are met:
 *
 * 3. Conditions 
 *
 * 3.1. Redistribution of Source with Rights to Further Distribute Source.  
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification with rights to further distribute source must include
 * the above Copyright Notice, the above License, this list of Conditions,
 * and the following Disclaimer and Export Compliance provision.  In addition,
 * Licensee must cause all Covered Code to which Licensee contributes to
 * contain a file documenting the changes Licensee made to create that Covered
 * Code and the date of any change.  Licensee must include in that file the
 * documentation of any changes made by any predecessor Licensee.  Licensee 
 * must include a prominent statement that the modification is derived,
 * directly or indirectly, from Original Intel Code.
 *
 * 3.2. Redistribution of Source with no Rights to Further Distribute Source.  
 * Redistribution of source code of any substantial portion of the Covered
 * Code or modification without rights to further distribute source must
 * include the following Disclaimer and Export Compliance provision in the
 * documentation and/or other materials provided with distribution.  In
 * addition, Licensee may not authorize further sublicense of source of any
 * portion of the Covered Code, and must include terms to the effect that the
 * license from Licensee to its licensee is limited to the intellectual
 * property embodied in the software Licensee provides to its licensee, and
 * not to intellectual property embodied in modifications its licensee may
 * make.
 *
 * 3.3. Redistribution of Executable. Redistribution in executable form of any
 * substantial portion of the Covered Code or modification must reproduce the
 * above Copyright Notice, and the following Disclaimer and Export Compliance
 * provision in the documentation and/or other materials provided with the
 * distribution.
 *
 * 3.4. Intel retains all right, title, and interest in and to the Original
 * Intel Code.
 *
 * 3.5. Neither the name Intel nor any other trademark owned or controlled by
 * Intel shall be used in advertising or otherwise to promote the sale, use or
 * other dealings in products derived from or relating to the Covered Code
 * without prior written authorization from Intel.
 *
 * 4. Disclaimer and Export Compliance
 *
 * 4.1. INTEL MAKES NO WARRANTY OF ANY KIND REGARDING ANY SOFTWARE PROVIDED
 * HERE.  ANY SOFTWARE ORIGINATING FROM INTEL OR DERIVED FROM INTEL SOFTWARE
 * IS PROVIDED "AS IS," AND INTEL WILL NOT PROVIDE ANY SUPPORT,  ASSISTANCE,
 * INSTALLATION, TRAINING OR OTHER SERVICES.  INTEL WILL NOT PROVIDE ANY
 * UPDATES, ENHANCEMENTS OR EXTENSIONS.  INTEL SPECIFICALLY DISCLAIMS ANY
 * IMPLIED WARRANTIES OF MERCHANTABILITY, NONINFRINGEMENT AND FITNESS FOR A
 * PARTICULAR PURPOSE. 
 *
 * 4.2. IN NO EVENT SHALL INTEL HAVE ANY LIABILITY TO LICENSEE, ITS LICENSEES
 * OR ANY OTHER THIRD PARTY, FOR ANY LOST PROFITS, LOST DATA, LOSS OF USE OR
 * COSTS OF PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES, OR FOR ANY INDIRECT,
 * SPECIAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THIS AGREEMENT, UNDER ANY
 * CAUSE OF ACTION OR THEORY OF LIABILITY, AND IRRESPECTIVE OF WHETHER INTEL
 * HAS ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.  THESE LIMITATIONS
 * SHALL APPLY NOTWITHSTANDING THE FAILURE OF THE ESSENTIAL PURPOSE OF ANY
 * LIMITED REMEDY.
 *
 * 4.3. Licensee shall not export, either directly or indirectly, any of this
 * software or system incorporating such software without first obtaining any
 * required license or other approval from the U. S. Department of Commerce or
 * any other agency or department of the United States Government.  In the
 * event Licensee exports any such software from the United States or
 * re-exports any such software from a foreign destination, Licensee shall
 * ensure that the distribution and export/re-export of the software is in
 * compliance with all laws, regulations, orders, or other restrictions of the
 * U.S. Export Administration Regulations. Licensee agrees that neither it nor
 * any of its subsidiaries will export/re-export any technical data, process,
 * software, or service, directly or indirectly, to any country for which the
 * United States government or any agency thereof requires an export license,
 * other governmental approval, or letter of assurance, without first obtaining
 * such license, approval or letter.
 *
 *****************************************************************************/


#define __NSAPIOBJ_C__

#include <acpi.h>
#include <interp.h>
#include <namesp.h>
#include <methods.h>
#include <acobject.h>
#include <pnp.h>


#define _COMPONENT          NAMESPACE
        MODULE_NAME         ("nsapiobj");


/****************************************************************************
 *
 * FUNCTION:    AcpiEvaluateObject
 *
 * PARAMETERS:  Handle              - Object handle (optional)
 *              *Pathname           - Object pathname (optional)
 *              **Params            - List of parameters to pass to
 *                                    method, terminated by NULL.
 *                                    Params itself may be NULL
 *                                    if no parameters are being
 *                                    passed.
 *              *ReturnObject       - Where to put method's return value (if 
 *                                    any).  If NULL, no value is returned.
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Find and evaluate the given object, passing the given
 *              parameters if necessary.  One of "Handle" or "Pathname" must
 *              be valid (non-null)
 *
 ****************************************************************************/

ACPI_STATUS
AcpiEvaluateObject (
    ACPI_HANDLE             Handle, 
    ACPI_STRING             Pathname, 
    ACPI_OBJECT_LIST        *ParamObjects,
    ACPI_BUFFER             *ReturnBuffer)
{
    ACPI_STATUS             Status;
    ACPI_OBJECT_INTERNAL    **ParamPtr = NULL;
    ACPI_OBJECT_INTERNAL    *ReturnObj = NULL;
    ACPI_OBJECT_INTERNAL    *ObjectPtr = NULL;
    UINT32                  BufferSpaceNeeded;
    UINT32                  UserBufferLength;
    UINT32                  Count;
    UINT32                  i;
    UINT32                  ParamLength;
    UINT32                  ObjectLength;


    FUNCTION_TRACE ("AcpiEvaluateObject");


    /* 
     * If there are parameters to be passed to the object (which must be a control method), 
     * the external objects must be converted to internal objects
     */

    if (ParamObjects && ParamObjects->Count)
    {
        /* 
         * Allocate a new parameter block for the internal objects
         * Add 1 to count to allow for null terminated internal list
         * TBD: merge into single allocation!
         */

        Count           = ParamObjects->Count;
        ParamLength     = (Count + 1) * sizeof (void *);
        ObjectLength    = Count * sizeof (ACPI_OBJECT_INTERNAL);

        ParamPtr = CmCallocate (ParamLength +       /* Parameter List part */
                                ObjectLength);      /* Actual objects */
        if (!ParamPtr)
        {
            return_ACPI_STATUS (AE_NO_MEMORY);
        }

        ObjectPtr = (ACPI_OBJECT_INTERNAL *) ((UINT8 *) ParamPtr + ParamLength);

        /*
         * Init the param array of pointers and NULL terminate the list
         */

        for (i = 0; i < Count; i++)
        {
            ParamPtr[i] = &ObjectPtr[i];
            CmInitStaticObject (&ObjectPtr[i]);
        }
        ParamPtr[Count] = NULL;                 

        /* 
         * Convert each external object in the list to an internal object
         */
        for (i = 0; i < Count; i++)
        {
            Status = CmBuildInternalObject (&ParamObjects->Pointer[i], ParamPtr[i]);
            if (ACPI_FAILURE (Status))
            {
                CmDeleteInternalObjectList (ParamPtr);
                return_ACPI_STATUS (Status);
            }
        }
    }


    /*
     * Three major cases:
     * 1) Fully qualified pathname 
     * 2) No handle, not fully qualified pathname (error)
     * 3) Valid handle 
     */

    if ((Pathname) && 
        (NsValidRootPrefix (Pathname[0])))
    {
        /*
         *  The path is fully qualified, just evaluate by name
         */
        Status = NsEvaluateByName (Pathname, ParamPtr, &ReturnObj);
    }

    else if (!Handle)
    {
        /*
         * A handle is optional iff a fully qualified pathname is specified.
         * Since we've already handled fully qualified names above, this is an error
         */

        if (!Pathname)
        {
            DEBUG_PRINT (ACPI_ERROR, ("AcpiEvaluateObject: Both Handle and Pathname are NULL\n"));
        }

        else
        {
            DEBUG_PRINT (ACPI_ERROR, ("AcpiEvaluateObject: Handle is NULL and Pathname is relative\n"));
        }

        Status = AE_BAD_PARAMETER;
    }

    else 
    {
        /*
         *  We get here if we have a handle -- and if we have a pathname it is relative.
         *  The handle will be validated in the lower procedures
         */

        if (!Pathname)
        {
            /*
             * The null pathname case means the handle is for the actual object to be evaluated
             */
            Status = NsEvaluateByHandle (Handle, ParamPtr, &ReturnObj);
        }

        else
        {
           /*
            * Both a Handle and a relative Pathname
            */
            Status = NsEvaluateRelative (Handle, Pathname, ParamPtr, &ReturnObj);
        }
    }


    
    /* 
     * If we are expecting a return value, and all went well above, 
     * copy the return value to an external object.
     */

    if (ReturnBuffer)
    {
        UserBufferLength = ReturnBuffer->Length;
        ReturnBuffer->Length = 0;

        if (ReturnObj)
        {
            if (VALID_DESCRIPTOR_TYPE (ReturnObj, DESC_TYPE_NTE))
            {
                /* 
                 * If we got an NTE as a return object, this means the object we are evaluating has nothing
                 * interesting to return (such as a mutex, etc.)  We return an error because these types
                 * are essentially unsupported by this interface.  We don't check up front because this makes
                 * it easier to add support for various types at a later date if necessary.
                 */
                Status = AE_TYPE;
                ReturnObj = NULL;   /* No need to delete an NTE */
            }

            if (ACPI_SUCCESS (Status))
            {
                /*
                 *  Find out how large a buffer is needed to contain the
                 *  returned object
                 */
                Status = CmGetObjectSize (ReturnObj, &BufferSpaceNeeded);
                if (ACPI_SUCCESS (Status))
                {
                    /* Check if there is enough room in the caller's buffer */

                    if (UserBufferLength < BufferSpaceNeeded) 
                    {
                        /*
                         *  Caller's buffer is too small, can't give him partial results
                         *  fail the call but return the buffer size needed
                         */

                        DEBUG_PRINT (ACPI_ERROR, ("AcpiEvaluateObject: Needed buffer size %d, received %d\n",
                                                    BufferSpaceNeeded, UserBufferLength));

                        ReturnBuffer->Length = BufferSpaceNeeded;
                        Status = AE_BUFFER_OVERFLOW;
                    }

                    else
                    {
                        /*
                         *  We have enough space for the object, build it
                         */
                        Status = CmBuildExternalObject (ReturnObj, ReturnBuffer);
                        ReturnBuffer->Length = BufferSpaceNeeded;
                    }
                }
            }
        }
    }


    /* Delete the return and parameter objects */

    if (ReturnObj)
    {
        /*
         * Delete the internal return object. (Or at least
         * decrement the reference count by one)
         */
        CmDeleteInternalObject (ReturnObj);
    }

    /*
     * Free the input parameter list (if we created one),
     */

    if (ParamPtr)
    {
        /* Free the allocated parameter block */

        CmDeleteInternalObjectList (ParamPtr);
    }

    return_ACPI_STATUS (Status);
}


/****************************************************************************
 *
 * FUNCTION:    AcpiGetNextObject
 *
 * PARAMETERS:  Type            - Type of object to be searched for
 *              Parent          - Parent object whose children we are getting
 *              LastChild       - Previous child that was found.  
 *                                The NEXT child will be returned
 *              RetHandle       - Where handle to the next object is placed
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Return the next peer object within the namespace.  If Handle is
 *              valid, Scope is ignored.  Otherwise, the first object within 
 *              Scope is returned.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiGetNextObject (
    ACPI_OBJECT_TYPE        Type, 
    ACPI_HANDLE             Parent, 
    ACPI_HANDLE             Child, 
    ACPI_HANDLE             *RetHandle)
{
    NAME_TABLE_ENTRY        *ThisEntry;
    ACPI_STATUS             Status = AE_NOT_FOUND;



    /* Parameter validation */

    if (Type > ACPI_TYPE_MAX)
    {
        return AE_BAD_PARAMETER;
    }

    CmAcquireMutex (MTX_NAMESPACE);

    /* If null handle, use the parent */

    if (!Child)
    {
        /* Start search at the beginning of the specified scope */

        ThisEntry = NsConvertHandleToEntry (Parent);
        if (!ThisEntry)
        {
            Status = AE_BAD_PARAMETER;
            goto UnlockAndExit;
        }

        /* It's really the parent's _scope_ that we want */

        ThisEntry = ThisEntry->Scope;
    }

    /* Non-null handle, ignore the parent */

    else
    {
        /* Convert and validate the handle */

        ThisEntry = NsConvertHandleToEntry (Child);
        if (!ThisEntry)
        {
            Status = AE_BAD_PARAMETER;
            goto UnlockAndExit;
        }

        /* Start search at the NEXT object */

        ThisEntry = ThisEntry->NextEntry;
    }



    /* If any type is OK, we are done */

    if (Type == ACPI_TYPE_Any)
    {
        /* Make sure this is valid entry first */

        if (!ThisEntry)
        {
            Status = AE_NOT_FOUND;
            goto UnlockAndExit;
        }

        if (!ThisEntry->Name)
        {
            Status = AE_NOT_FOUND;
            goto UnlockAndExit;
        }

        if (RetHandle)
        {
            *RetHandle = NsConvertEntryToHandle (ThisEntry);
        }

        Status = AE_OK;
        goto UnlockAndExit;
    }


    /* Must search -- entries within this scope only */

    while (ThisEntry)
    {
        /* If type matches, we are done */

        if (ThisEntry->Type == Type)
        {
            if (RetHandle)
            {
                *RetHandle = NsConvertEntryToHandle (ThisEntry);
            }

            Status = AE_OK;
            goto UnlockAndExit;
        }

        /* Otherwise, move on to the next object */

        ThisEntry = ThisEntry->NextEntry;
    }


UnlockAndExit:

    CmReleaseMutex (MTX_NAMESPACE);
    return Status;
}


/****************************************************************************
 *
 * FUNCTION:    AcpiGetType
 *
 * PARAMETERS:  Handle          - Handle of object whose type is desired
 *              *RetType        - Where the type will be placed
 *
 * RETURN:      Status
 *
 * DESCRIPTION: This routine returns the type associatd with a particular handle
 *
 ******************************************************************************/

ACPI_STATUS 
AcpiGetType (
    ACPI_HANDLE             Handle,
    ACPI_OBJECT_TYPE        *RetType)
{
    NAME_TABLE_ENTRY        *Object;


    /* Parameter Validation */

    if (!RetType)
    {
        return AE_BAD_PARAMETER;
    }

    /* Special case for the predefined Root Object (return type ANY) */

    if (Handle == ACPI_ROOT_OBJECT)
    {
        *RetType = ACPI_TYPE_Any;
        return AE_OK;
    }

    CmAcquireMutex (MTX_NAMESPACE);

    /* Convert and validate the handle */

    Object = NsConvertHandleToEntry (Handle);
    if (!Object)
    {
        CmReleaseMutex (MTX_NAMESPACE);
        return AE_BAD_PARAMETER;
    }

    *RetType = Object->Type;


    CmReleaseMutex (MTX_NAMESPACE);
    return AE_OK;
}


/****************************************************************************
 *
 * FUNCTION:    AcpiGetParent
 *
 * PARAMETERS:  Handle          - Handle of object whose parent is desired
 *              RetHandle       - Where the parent handle will be placed
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Returns a handle to the parent of the object represented by
 *              Handle.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiGetParent (
    ACPI_HANDLE             Handle, 
    ACPI_HANDLE             *RetHandle)
{
    NAME_TABLE_ENTRY        *Object;
    ACPI_STATUS             Status = AE_OK;
    

    /* No trace macro, too verbose */


    if (!RetHandle)
    {
        return AE_BAD_PARAMETER;
    }

    /* Special case for the predefined Root Object (no parent) */

    if (Handle == ACPI_ROOT_OBJECT)
    {
        return AE_NULL_ENTRY;
    }


    CmAcquireMutex (MTX_NAMESPACE);

    /* Convert and validate the handle */

    Object = NsConvertHandleToEntry (Handle);
    if (!Object)
    {
        Status = AE_BAD_PARAMETER;
        goto UnlockAndExit;
    }

   
    /* Get the parent entry */

    *RetHandle = NsConvertEntryToHandle(Object->ParentEntry);

    /* Return exeption if parent is null */

    if (!Object->ParentEntry)
    {
        Status = AE_NULL_ENTRY;
    }


UnlockAndExit:

    CmReleaseMutex (MTX_NAMESPACE);
    return AE_OK;
}


/******************************************************************************
 *
 * FUNCTION:    AcpiWalkNamespace
 *
 * PARAMETERS:  Type                - ACPI_OBJECT_TYPE to search for
 *              StartObject         - Handle in namespace where search begins
 *              MaxDepth            - Depth to which search is to reach
 *              UserFunction        - Called when an object of "Type" is found
 *              Context             - Passed to user function
 *
 * RETURNS      Return value from the UserFunction if terminated early.
 *              Otherwise, returns NULL.
 *
 * DESCRIPTION: Performs a modified depth-first walk of the namespace tree,
 *              starting (and ending) at the object specified by StartHandle.
 *              The UserFunction is called whenever an object that matches
 *              the type parameter is found.  If the user function returns
 *              a non-zero value, the search is terminated immediately and this
 *              value is returned to the caller.
 *
 *              The point of this procedure is to provide a generic namespace
 *              walk routine that can be called from multiple places to 
 *              provide multiple services;  the User Function can be tailored
 *              to each task, whether it is a print function, a compare 
 *              function, etc.
 *
 ******************************************************************************/

ACPI_STATUS
AcpiWalkNamespace (
    ACPI_OBJECT_TYPE        Type, 
    ACPI_HANDLE             StartObject, 
    UINT32                  MaxDepth,
    WALK_CALLBACK           UserFunction, 
    void                    *Context, 
    void                    **ReturnValue)
{
    ACPI_STATUS             Status;
    ACPI_HANDLE             ChildHandle;
    ACPI_HANDLE             ParentHandle;
    ACPI_OBJECT_TYPE        ChildType;
    UINT32                  Level;


    FUNCTION_TRACE ("AcpiWalkNamespace");


    /* Parameter validation */

    if ((Type > ACPI_TYPE_MAX) ||
        (!MaxDepth)             || 
        (!UserFunction))
    {
        return_ACPI_STATUS (AE_BAD_PARAMETER);
    }

    /* Special case for the namespace root object */

    if (StartObject == ACPI_ROOT_OBJECT)
    {
        StartObject = Gbl_RootObject;
    }


    /* Null child means "get first object" */

    ParentHandle    = StartObject;
    ChildHandle     = 0;
    ChildType       = ACPI_TYPE_Any;
    Level           = 1;

    /* 
     * Traverse the tree of objects until we bubble back up to where we
     * started. When Level is zero, the loop is done because we have 
     * bubbled up to (and passed) the original parent handle (StartHandle)
     */

    while (Level > 0)
    {
        /* Get the next typed object in this scope.  Null returned if not found */

        Status = AE_OK;
        if (ACPI_SUCCESS (AcpiGetNextObject (ACPI_TYPE_Any, ParentHandle, ChildHandle, &ChildHandle)))
        {
            /* Found an object, Get the type if we are not searching for ANY */

            if (Type != ACPI_TYPE_Any)
            {
                AcpiGetType (ChildHandle, &ChildType);
            }

            if (ChildType == Type)
            {
                /* Found a matching object, invoke the user callback function */

                Status = UserFunction (ChildHandle, Level, Context, ReturnValue);
                switch (Status)
                {
                case AE_OK:
                case AE_DEPTH:
                    break;                          /* Just keep going */

                case AE_TERMINATE:
                    return_ACPI_STATUS (AE_OK);     /* Exit now, with OK status */
                    break;

                default:
                    return_ACPI_STATUS (Status);    /* All others are valid exceptions */
                    break;
                }
            }

            /* 
             * Depth first search:
             * Attempt to go down another level in the namespace if we are allowed to.
             * Don't go any further if we have reached the caller specified maximum depth
             * or if the user function has specified that the maximum depth has been reached.
             */

            if ((Level < MaxDepth) && (Status != AE_DEPTH))
            {
                if (ACPI_SUCCESS (AcpiGetNextObject (ACPI_TYPE_Any, ChildHandle, 0, NULL)))
                {
                    /* There is at least one child of this object, visit the object */

                    Level++;
                    ParentHandle    = ChildHandle;
                    ChildHandle     = 0;
                }
            }
        }

        else
        {
            /* 
             * No more children in this object (AcpiGetNextObject failed), 
             * go back upwards in the namespace tree to the object's parent.
             */
            Level--;
            ChildHandle = ParentHandle;
            AcpiGetParent (ParentHandle, &ParentHandle);
        }
    }


    return_ACPI_STATUS (AE_OK);                   /* Complete walk, not terminated by user function */
}



/* NON-IMPLEMENTED FUNCTIONS */


/****************************************************************************
 *
 * FUNCTION:    AcpiGetObject
 *
 * PARAMETERS:  
 *
 * RETURN:      Status
 *
 * DESCRIPTION: TBD: Not completely defined, not implemented!
 *
 ******************************************************************************/

ACPI_STATUS
AcpiGetObject (
    ACPI_STRING             *Pathname, 
    ACPI_HANDLE             *RetHandle)
{

    if (!RetHandle)
    {
        return AE_BAD_PARAMETER;
    }


    *RetHandle = Gbl_RootObject->Scope;
    return AE_OK;
}


/******************************************************************************
 *
 * FUNCTION:    AcpiSetFirmwareWakingVector
 *
 * PARAMETERS:  PhysicalAddress     - Physical address of ACPI real mode
 *                                          entry point.
 *
 * RETURN:      AE_OK or AE_ERROR
 *
 * DESCRIPTION: Access function for dFirmwareWakingVector field in FACS
 *
 ******************************************************************************/

ACPI_STATUS
AcpiSetFirmwareWakingVector (
    void                    *PhysicalAddress)
{
    FUNCTION_TRACE ("AcpiSetFirmwareWakingVector");
    
    return_ACPI_STATUS ((AE_OK));
}


/******************************************************************************
 *
 * FUNCTION:    AcpiGetFirmwareWakingVector
 *
 * PARAMETERS:  *PhysicalAddress    - Output buffer where contents of
 *                                          the dFirmwareWakingVector field of 
 *                                          the FACS will be stored.
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Access function for dFirmwareWakingVector field in FACS
 *
 ******************************************************************************/

ACPI_STATUS
AcpiGetFirmwareWakingVector (
    void                    **PhysicalAddress)
{
    FUNCTION_TRACE ("AcpiGetFirmwareWakingVector");

    
    return_ACPI_STATUS ((AE_OK));
}

