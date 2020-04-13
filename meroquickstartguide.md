# Mero QuickStart guide
This is a step by step guide to get the mero ready for you on your system.
Before jumping on cloning, make sure, you have your VMs setup with specifications mentioned in [Virtual Machine](VIRTUAL_MACHINE.md).

## Accessing the code right way
(For phase 1) Latest code which is getting evolved, advancing and contributed is on the gerrit server.
Seagate contributor will be refernecing, cloning and committing code to/from this [Gerrit server](http://gerrit.mero.colo.seagate.com:8080).

Following steps will make your access to server hassel free.

1. Create SSH Public Key
  * [SSH generation](https://git-scm.com/book/en/v2/Git-on-the-Server-Generating-Your-SSH-Public-Key) will make your key generation super easy. follow the instructions throughly.
2. Add SSH Public Key on [Gerrit server](http://gerrit.mero.colo.seagate.com:8080).
  * Log into the gerrit server with your seagate gid based credentials.
  * On right top corner you will see your name, open drop down menu by clicking and choose settings.
  * In the menu on left, click SSH Public Keys, and add your public key (which is generated in step one) right there.

WoW! :sparkles:
You are all set to fetch mero repo now. 

## Cloning Mero
Getting mero on your system is nothing but firing of below listed commands. let's jump into it.
1. $cd path/to/your/dev/directory
2. git clone --recursive ssh://g<yourGID>@gerrit.mero.colo.seagate.com:29418/mero
  * Make sure your replace <yourGID> with your seagate ID number.
3. Enable some pre-commit hooks required before pushing your changes to remote (command to be run from the parent dir of Mero source).
  * cd mero && scp -p -P 29418 g<yourGId>@gerrit.mero.colo.seagate.com:hooks/commit-msg .git/hooks
4. build dependencies
  * $ cd mero
  * $ ./scripts/install-build-deps (TBD incase of packages are not present)

## Compiliation and Running Unit Test
All following commands assumes that user is already into it's main source directory.
1. building mero
  * ./scripts/m0 make
2. Running Unit Tests (UTs)
  * sudo ./scripts/m0 run-ut
    > Feel free to expore other options of this run-ut command. Try : sudo run-ut --help
    
    > sudo access is necessary as inserts relevant kernel modules beforehand.
3. For kernel space UTs
  * sudo ./scripts/m0 run-kut
4. Running a system test
    
   As an example for clovis module system test can be run using following command :
  * sudo ./clovis/st/utils/clovis_sync_replication_st.sh
  
KABOOM!!!
  
#### you're all set

You're awesome.

Let's start without a delay to contribute to seagate's open source initiative and join this movement with us keeping a common goal of making data storage better, more efficient and more accessible.

Seagate welcomes You! :relaxed:

