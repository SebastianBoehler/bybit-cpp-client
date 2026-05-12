std::string create_strategy_order(const std::string& json_body);
std::string get_strategy_list(const QueryParams& filters = {});
std::string get_strategy_order_list(const QueryParams& filters);
std::string stop_strategy(const std::string& strategy_id);
