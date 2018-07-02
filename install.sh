
      #########.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##."

HERE=$(cd $(dirname "$0");pwd)

# Check if Cutrun is currently installed
if [ $(command -v cutrun) ];
then
    echo "ERROR: Cutrun already installed at '$(cutrun --print-install-path)'."
    echo "       To uninstall, run:"
    echo '       rm -rf $(cutrun --print-install-path) $(which cutrun)'
    exit 1
else
    # Remove potentially broken symlink
    rm -rf /usr/local/bin/cutrun
fi


# Check if `remote` argument was passed to the script
if [ "$1" == "remote" ];
then
    # Install from Github repository
    git clone --depth=1 https://github.com/kube/cut /usr/local/share/cut
    ln -s /usr/local/share/cut/cutrun/cutrun.rb /usr/local/bin/cutrun
else
    # Install from local clone
    ln -s $HERE/cutrun/cutrun.rb /usr/local/bin/cutrun
fi


# Check that Cutrun was successfully installed
if [ $(command -v cutrun) ];
then
    echo "Cutrun successfully installed at $(cutrun --print-install-path)"
else
    echo "Error during installation"
fi
