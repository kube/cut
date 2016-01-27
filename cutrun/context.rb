
      #########.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##."

require 'test_source_file'

module CutRun
  class Context
    attr_reader :include_path
    attr_reader :out_folder

    def initialize options
      @libraries    = options['libraries']
      @library_path = options['library_path']
      @include_path = options['include_path']
      @out_folder   = options['out_folder']

      system "mkdir -p #{@out_folder}"

      # Fetch and parse tests source files
      @source_files = options['files'].map do |path|
        TestSourceFile.new self, path
      end
    end

    def add_library_path path
      @library_path << path
    end

    def add_library name
      @libraries << name
    end

    def run
      compile
      system "#{@out_folder}/cutrun"
    end

    def compile
      command = ["cc"]

      command += @library_path.map {|path| "-L#{path}"}
      command += @libraries.map {|name| "-l#{name}"}
      command += @source_files.map {|source| source.object_path}

      command << "-o #{@out_folder}/cutrun"

      system command*' '
    end
  end
end
