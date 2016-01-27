
      #########.
     ########",#:
   #########',##".
  ##'##'## .##',##.
   ## ## ## # ##",#.
    ## ## ## ## ##'
     ## ## ## :##
      ## ## ##."

module ArgumentsParser

  def self.flag? arg
    arg.start_with? '-'
  end

  def self.get_flag_name arg
    arg.start_with?('-') ? arg[0..1] : arg
  end

  def self.get_flag_value flag
    value = ARGV.shift

    if value.nil? or flag? value
      abort "Expected value for flag #{flag}"
    end

    return value
  end

  def self.get_complete_flag_name flag
    aliases = {
      '-I' => '--include-path',
      '-L' => '--library-path',
      '-l' => '--library'
    }
    aliases[flag] ? aliases[flag] : flag
  end

  def self.parse
    # Default options values
    options = {
      'files' => [],
      'libraries' => [],
      'include_path' => [],
      'library_path' => [],
      'out_folder' => './.cutrun_objects/'
    }

    # Parse all arguments
    loop do
      argument = ARGV.shift
      break if argument.nil?

      if flag? argument
        flag_name = get_flag_name argument

        case get_complete_flag_name flag_name
          when '--include-path'
            flag_value = get_flag_value argument
            options['include_path'] << flag_value

          when '--library-path'
            flag_value = get_flag_value argument
            options['library_path'] << flag_value

          when '--library'
            flag_value = get_flag_value argument
            options['libraries'] << flag_value

          else
            abort "Unknown flag #{flag_name}"
        end

      else
        options['files'] << argument
      end
    end

    options
  end
end
