README-:

If you extracted the files to a location other than C:\ProjectX then continue reading. Otherwise simply add the registry files to your registry by double clicking on them.

You must edit registry2.reg and registry3.reg to correctly point to the folder where you extracted ProjectX before you add them to your registry.

At the moment they are set for C:\ProjectX.

Note: each folder is delimited by "\\"

For registry2.reg update:

"CurrentDirectory"="C:\\ProjectX"
"Path"="C:\\ProjectX"

For registry3.reg update:

"Path"="C:\\ProjectX\\"
@="C:\\ProjectX\\ProjectX___Win32_Debug.exe"