**DFC data.win dumper Pin tool**

Download Pin v3.30  
<https://www.intel.com/content/www/us/en/developer/articles/tool/pin-a-binary-instrumentation-tool-downloads.html>

Copy the entire dfc_dump directory to
```
[PINROOT]\source\tools\dfc_dump
```

Open dfc_dump.sln, and build for Release x86.

Use the tool to dump data.win
```
[PINROOT]\pin.exe -t [PINROOT]\source\tools\dfc_dump\Release\dfc_dump.dll -o data.win -- "C:\path\to\DF CONNECTED v2.7.9c.exe"
```

This will open the game exe and it will run quite slowly, let it reach the main game screen, then close the game, and it will dump the decrypted data.win file.
