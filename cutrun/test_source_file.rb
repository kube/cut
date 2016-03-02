
      #########.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##."

require 'pathname'
require 'digest/sha1'
require 'directive'

module CutRun
  class TestSourceFile
    attr_reader :path
    attr_reader :object_path

    def initialize context, path
      @context = context
      @path = Pathname.new path
      @include_path = context.include_path

      parse
      compile
    end

    def add_include_path path
      @include_path << path
    end

    def add_library_path path
      @context.add_library_path path
    end

    def add_library name
      @context.add_library name
    end

    def parse
      IO.foreach(@path)
        .select do |line|
          Directive.is line
        end
        .each do |line|
          Directive.new self, line
        end
    end

    def compile
      output = Digest::SHA1.hexdigest(@path.to_path)
      @object_path = "#{@context.out_folder}/#{output}.o"

      command = [
        "cc",
        "-DCUT_SINGLE_FILE=1",
         "#{@path}"
      ]

      command += @include_path.map {|path| "-I#{path}"}

      command << "-DCUT_REPORTER_HEADER=\"\\\"reporters/cut_#{@context.options['reporter']}_reporter.h\\\"\""

      if @context.options['only_expand_macros'] == true
        command << "-E"
      else
        command << "-c"
        command << "-o #{@object_path}"
      end

      system command*' '
    end
  end
end
