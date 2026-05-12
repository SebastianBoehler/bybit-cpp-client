std::string get_institutional_loan_product_info(const QueryParams& filters = {}, bool authenticated = false);
std::string get_institutional_loan_margin_coin_info(const QueryParams& filters = {}, bool authenticated = false);
std::string get_institutional_loan_orders(const QueryParams& filters = {});
std::string get_institutional_loan_repayment_orders(const QueryParams& filters = {});
std::string get_institutional_loan_ltv();
std::string bind_or_unbind_institutional_loan_uid(const std::string& json_body);
std::string repay_institutional_loan(const std::string& json_body);
