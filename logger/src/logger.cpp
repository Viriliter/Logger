#include <logger.h>

using namespace logger;

/*********************************************************************
 * 
 * Static Variables 
 * 
*********************************************************************/

Logger *Logger::ptr_instance_{nullptr};
std::mutex Logger::mutex_;
structLogFormat Logger::fmt_{};

std::string Logger::config_file_path_ = "";
std::string Logger::out_file_dir_ = "";
std::string Logger::out_filename_ = "";
std::string Logger::root_filename_ = "";

enumLogLevel Logger::current_level_ = enumLogLevel::INFO_;

bool Logger::is_output_set_ = false;
bool Logger::is_configure_set_ = false;
bool Logger::is_thread_safe_ = false;
bool Logger::color_enabled_ = false;
bool Logger::is_source_enabled_ = false;

uint64_t Logger::err_counter_ = 0;

std::ofstream Logger::log_file_{};

std::string Logger::log_source_file_ = "";
int Logger::log_source_line_ = 0;

std::map<enumLogLevel, structLogLevel> Logger::mapLogLevel{
    {enumLogLevel::FATAL_, {"FATAL", std::string(COLOR_MAGENTA) }},
    {enumLogLevel::ERROR_, {"ERROR", std::string(COLOR_RED) }},
    {enumLogLevel::ALERT_, {"ALERT", std::string(COLOR_BRIGHT_YELLOW) }},
    {enumLogLevel::WARNING_, {"WARNING", std::string(COLOR_YELLOW) }},
    {enumLogLevel::INFO_, {"INFO", std::string(COLOR_WHITE) }},
    {enumLogLevel::DEBUG_, {"DEBUG", std::string(COLOR_CYAN), SHOW_SOURCE_INFO }},
    {enumLogLevel::TRACE_, {"TRACE", std::string(COLOR_GREEN), SHOW_SOURCE_INFO }},
    {enumLogLevel::INVALID_, {"INVALID", std::string(COLOR_RESET) }},
    {enumLogLevel::LOG_ERROR_, {"LOG ERROR",  std::string(COLOR_BG_RED) }}
};

bool Logger::timestamp_suffix_enabled_ = false;
structTimestamp Logger::currentTimestamp_{};

/*********************************************************************
 * 
 * Private Functions 
 * 
*********************************************************************/

std::string Logger::log_out_(const structLogMsg &msg_log){
    std::string out = "";
    
    std::string str_timestamp = format_time(msg_log.timestamp, fmt_.fmt_timestamp);
    add_field(str_timestamp, out, fmt_);
    if (msg_log.log_level_desc == ""){
            add_field(mapLogLevel[msg_log.log_level].desc, out, fmt_);
    }
    else{
            add_field(msg_log.log_level_desc, out, fmt_);
    } 
    add_field(msg_log.source, out, fmt_);
    add_field(msg_log.msg, out, fmt_);
    add_newline(out);
    
    lstrip_delimiter(out, fmt_);
    return out;
}

void Logger::write_console_(const structLogMsg &msg_log){
    std::string out = "";
    out = log_out_(msg_log);

    if (color_enabled_)
        std::cout << pick_log_color_(msg_log.log_level) << out << COLOR_RESET;
    else
        std::cout << out;
    std::cout.flush();
}

void Logger::write_file_(const structLogMsg &msg_log){
    if (out_filename_ == "") return;

    std::string out = "";
    out = log_out_(msg_log);

    if(timestamp_suffix_enabled_){

        if(currentTimestamp_.tm_year == msg_log.timestamp.tm_year &&
           currentTimestamp_.tm_mon  == msg_log.timestamp.tm_mon &&
           currentTimestamp_.tm_mday == msg_log.timestamp.tm_mday){
            // No change in date
        }
        else{
            // Update current timestamp on the change of date
            currentTimestamp_ = msg_log.timestamp;
            out_filename_ = add_timestamp_prefix_(msg_log.timestamp);
        }
    }
    // Open file in append mode
    log_file_.open(out_file_dir_ + out_filename_, std::ios::app);
    if(log_file_.is_open()) log_file_  << out;
    else error_handler_("Log file cannot be opened.");
    log_file_.close();
}

void Logger::error_handler_(const std::string &err_msg){
    structTimestamp ts;
    get_current_timestamp_struct(ts);
    
    set_log_level(enumLogLevel::LOG_ERROR_);

    if (err_counter_<UINT64_MAX) err_counter_++;

    structLogMsg err_log;
    err_log.timestamp = ts;
    err_log.log_level = current_level_;
    err_log.log_level_desc = "[" + mapLogLevel[current_level_].desc + " " + std::to_string(err_counter_) + "]";
    
    err_log.source = ((bool) ((mapLogLevel[current_level_].option & MASK_SHOW_SOURCE_INFO) != 0) && is_source_enabled_)? log_source_file_ + ":" + std::to_string(log_source_line_): "";
    err_log.msg = std::string(err_msg);
    
    std::string out = "";
    out = log_out_(err_log);

    if (color_enabled_){
        out = pick_log_color_(err_log.log_level) + out;
        if (out.size() > 0) out.insert(out.size()-1, COLOR_RESET);
        std::cout << out << COLOR_RESET;
    }
    else{
        std::cout << out;
    }
    std::cout.flush();
}

inline std::string Logger::pick_log_color_(enumLogLevel log_level){
    for(auto &elem: mapLogLevel){
        if (log_level == elem.first){
            return elem.second.color;
        }
    }
    return mapLogLevel[enumLogLevel::INVALID_].color;
}

inline std::string Logger::add_timestamp_prefix_(structTimestamp ts){
    std::string result;
    std::string timestamp_prefix = "";
    timestamp_prefix = format_time(ts, DEFAULT_TIMESTAMP_PREFIX_FORMAT);
    if (root_filename_.empty()){
        std::string format;
        get_file_format(root_filename_, format);
        format = format.empty()? std::string(DEFAULT_LOG_EXTENSION): format;
        result = timestamp_prefix + format;
    }
    else{
        result = timestamp_prefix + "_" + root_filename_;
    }
    return result;
}

void Logger::set_output_(const std::string &filename, const std::string file_dir, bool timestamp_prefix_enabled){
    std::string temp_out_filename = "";
    root_filename_ = std::string(filename);
    std::string file_format;

    if (timestamp_prefix_enabled){
        structTimestamp ts;
        get_current_timestamp_struct(ts);
        temp_out_filename = add_timestamp_prefix_(ts);
    }
    else{
        temp_out_filename = root_filename_;
    }

    // Check empty filename
    if (temp_out_filename.empty()){
        error_handler_("Filename is empty.");
        return;
    }

    // Check file extension if nothing is provided set to default format (.log).
    auto r = get_file_format(temp_out_filename, file_format);
    if (r<0){
        temp_out_filename += DEFAULT_LOG_EXTENSION;
    }
    // Check filename only contains with file extension
    else if (r==0){
        error_handler_("Filename is not valid: " + temp_out_filename);
        return;
    }
    else{}
    
    // If the file directory is not provided, default log directory is home location
    std::string temp_out_file_dir = (file_dir=="")? HomeDir: std::string(file_dir);
    // Append "/" to the end of directory path
    if (temp_out_file_dir.back() != '/') temp_out_file_dir.push_back('/');
    // Convert all \ chars to / for cross-platfrom portatibility
    std::replace(temp_out_file_dir.begin(), temp_out_file_dir.end(), '\\', '/');

    log_file_.open(temp_out_file_dir + temp_out_filename, std::ios::out);
    if(!log_file_.is_open()){
        error_handler_("The log file cannot be opened.");
        return;
    }
    log_file_.close();

    out_filename_ = temp_out_filename;
    out_file_dir_ = temp_out_file_dir;
    is_output_set_ = true;
}

void Logger::set_format_(structLogFormat &fmt){
    fmt_.fmt_delimiter_type = fmt.fmt_delimiter_type;
    fmt_.fmt_padding_size = fmt.fmt_padding_size;
    fmt_.fmt_timestamp = fmt.fmt_timestamp;
}

/*********************************************************************
 * 
 * Protected Functions 
 * 
*********************************************************************/

Logger::Logger() {
    structLogFormat fmt;
    fmt.fmt_delimiter_type = DEFAULT_DELIMITER_TYPE;
    fmt.fmt_padding_size = DEFAULT_PADDING_SIZE;
    fmt.fmt_timestamp = DEFAULT_TIMESTAMP_FORMAT;
    set_format_(fmt);
}

Logger::~Logger(){
    if(log_file_.is_open()) log_file_.close();

    delete ptr_instance_;
    ptr_instance_ = NULL;
}

/*********************************************************************
 * 
 * Public Functions 
 * 
*********************************************************************/

Logger *Logger::getInstance(){
    if(is_thread_safe_) std::lock_guard<std::mutex> lock(mutex_);

    if (ptr_instance_ == nullptr){
        ptr_instance_ = new Logger();
        return ptr_instance_;
    }
    else{
        return ptr_instance_;
    }
}

Logger *Logger::operator<<(char *s_){
    structLogMsg log;
    structTimestamp ts;
    get_current_timestamp_struct(ts);
    
    log.timestamp = ts;
    log.log_level = current_level_;
    log.source = ((bool) ((mapLogLevel[current_level_].option & MASK_SHOW_SOURCE_INFO) != 0) && is_source_enabled_)? log_source_file_ + ":" + std::to_string(log_source_line_): "";
    log.msg = std::string(s_);

    write_console_(log);
    write_file_(log);

    return ptr_instance_;
}

Logger *Logger::operator<<(const std::string &s_){
    structLogMsg log;
    structTimestamp ts;
    get_current_timestamp_struct(ts);
    
    log.timestamp = ts;
    log.log_level = current_level_;
    log.source = ((bool) ((mapLogLevel[current_level_].option & MASK_SHOW_SOURCE_INFO) != 0) && is_source_enabled_)? log_source_file_ + ":" + std::to_string(log_source_line_): "";
    log.msg = s_;

    write_console_(log);
    write_file_(log);

    return ptr_instance_;
}

Logger *Logger::operator<<(std::string &s_){
    structLogMsg log;
    structTimestamp ts;
    get_current_timestamp_struct(ts);
    
    log.timestamp = ts;
    log.log_level = current_level_;
    log.source = ((bool) ((mapLogLevel[current_level_].option & MASK_SHOW_SOURCE_INFO) != 0) && is_source_enabled_)? log_source_file_ + ":" + std::to_string(log_source_line_): "";
    log.msg = s_;

    write_console_(log);
    write_file_(log);

    return ptr_instance_;
}

Logger *Logger::operator<<(const char *s_){
    structLogMsg log;
    structTimestamp ts;
    get_current_timestamp_struct(ts);
    
    log.timestamp = ts;
    log.log_level = current_level_;
    log.source = ((bool) ((mapLogLevel[current_level_].option & MASK_SHOW_SOURCE_INFO) != 0) && is_source_enabled_)? log_source_file_ + ":" + std::to_string(log_source_line_): "";
    log.msg = std::string(s_);

    write_console_(log);
    write_file_(log);

    return ptr_instance_;
}

void Logger::configure(const std::string file_path){
    if (is_configure_set_){
        error_handler_("Configuration file can be set only once.");
        return;
    }

    config_file_path_ = file_path;
    is_configure_set_ = true;
}

void Logger::configure(char *file_path){
    if (is_configure_set_){
        error_handler_("Configuration file can be set only once.");
        return;
    }

    config_file_path_ = std::string(file_path);
    is_configure_set_ = true;
}

Logger *Logger::set_log_level(enumLogLevel level, const char *file, int line) noexcept{
    current_level_ = level;
    
    log_source_file_ = (file==nullptr)? log_source_file_ : std::string(file);
    log_source_line_ = (line<0)? log_source_line_ : line;

    return ptr_instance_;
}

void Logger::set_output(const std::string &filename, const std::string file_dir, bool timestamp_prefix_enabled){
    if (is_output_set_){
        error_handler_("Output file can be set only once.");
        return;
    };

    set_output_(filename, file_dir, timestamp_prefix_enabled);
}

void Logger::set_output(const char *filename, const char *file_dir, bool timestamp_prefix_enabled){
    if (is_output_set_){
        error_handler_("Output file can be set only once.");
        return;
    };
    
    std::string temp_out_filename_ = std::string(filename);
    // If file directory is not provided, default log directory is home location
    std::string temp_out_file_dir_ = file_dir==nullptr? "": std::string(file_dir);
    set_output_(temp_out_filename_, temp_out_file_dir_, timestamp_prefix_enabled);
}

void Logger::enable_colors() noexcept{
    color_enabled_ = true;
}

void Logger::disable_colors() noexcept{
    color_enabled_ = false;            
}

void Logger::set_thread_safety(bool is_safe) noexcept{
    is_thread_safe_ = is_safe;            
}

void Logger::enable_source() noexcept{
    is_source_enabled_ = true;
}

void Logger::disable_source() noexcept{
    is_source_enabled_ = false;
}

std::string Logger::get_log_path() noexcept{
    return out_file_dir_ + out_filename_;
}
