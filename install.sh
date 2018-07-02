
      #########.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##."

HERE=$(cd $(dirname "$0");pwd)

# Check if `remote` argument was passed to the script
if [ "$1" = "remote"];
then
    # Install from Github repository
    git clone --depth=1 git@github.com:kube/cut /usr/local/share
    ln -s /usr/local/share/cut/cutrun/cutrun.rb /usr/local/bin/cutrun
else
    # Install from local clone
    ln -s $HERE/cutrun/cutrun.rb /usr/local/bin/cutrun
fi


