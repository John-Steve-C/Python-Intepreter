#include "Evalvisitor.h"

//todo:用 x 去构造对应的、正确的 New_Any
New_Any EvalVisitor::Query(antlrcpp::Any x){
    //先判断x是不是变量名，不是就直接构造
    if (! x.is<string>()) { return x.as<New_Any>(); }
    //再去查询该变量是否存在
    auto temp = scope.varQuery(x.as<string>() );//pair
    if ( temp.first ) {//x对应的变量是否存在
        return temp.second;
    }
    else return New_Any();
}

antlrcpp::Any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
    return visitChildren(ctx);
}

//todo:函数声明
antlrcpp::Any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
    string name = ctx->NAME()->getText();

    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
    // no func def
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
    return visitChildren(ctx);
}

//todo:赋值语句
antlrcpp::Any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
    auto testlistArray = ctx->testlist();//vector
    if (testlistArray.size() == 1) {
        visitTestlist(testlistArray[0]);//dfs
        return 0;
    }

//    auto right_list = visitTestlist(testlistArray.back()).as<vector<antlrcpp::Any> >();
    if (ctx->augassign()) {//复合运算符"+="
        std::string op = ctx->augassign()->getText();
        std::string varName = testlistArray[0]->getText();

        New_Any varData = Query( visitTestlist(testlistArray[1]) );
//        bool flag = scope.varQuery(varName).first;//判断变量是否存在

        if (op == "+=") {
            scope.varRegister(varName, varData + scope.varQuery(varName).second);//存储变量
        } else if (op == "-=") {
            scope.varRegister(varName, scope.varQuery(varName).second - varData);
        } else if (op == "*=") {
            scope.varRegister(varName, varData * scope.varQuery(varName).second);
        } else if (op == "//=" || op == "/=") {
            scope.varRegister(varName, scope.varQuery(varName).second / varData);
        } else if (op == "%=") {
            scope.varRegister(varName, scope.varQuery(varName).second % varData);
        }

        return 0;
    }

    if (testlistArray.size() > 2) {//连等运算
        New_Any varData = Query( visitTestlist(testlistArray.back()) );//返回的类型变成int?
        for (auto i: testlistArray) {
            std::string varName = i->getText();
            scope.varRegister(varName, varData);
        }
        return 0;
    }

    std::string varName = testlistArray[0]->getText();
    //注意到，visitTestlist(testlistArray[1])返回的Any不一定是 变量名
    New_Any varData = Query( visitTestlist(testlistArray[1]) );
    scope.varRegister(varName, varData);//存储变量，默认变量名合法
    return 0;
}

antlrcpp::Any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
    return visitChildren(ctx);
}

//todo:if分支和 while循环
antlrcpp::Any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
    return visitChildren(ctx);
}

//todo:逻辑运算（与或非）
antlrcpp::Any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
    bool ans = false;
    auto and_list = ctx->and_test();
    if (and_list.size() == 1) return visitAnd_test(and_list[0]); //递归遍历(dfs)
    for (auto i:and_list){
        ans = ans || visitAnd_test(i).as<bool>(); // need to change！
        if (ans) return New_Any(true); //剪枝
    }

    return New_Any(false);
}

antlrcpp::Any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
    bool ans = true;
    auto not_list = ctx->not_test();
    if (not_list.size() == 1) return visitNot_test(not_list[0]);
    for (auto i:not_list){
        ans = ans && visitNot_test(i).as<bool>();
        if (!ans) return New_Any(false);
    }

    return New_Any(true);
}

antlrcpp::Any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
    if (ctx->comparison()) return visitComparison(ctx->comparison());

    return (New_Any) !visitNot_test(ctx->not_test()).as<bool>();
}

//todo:比较大小,返回真假
antlrcpp::Any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
    auto arithArray = ctx->arith_expr();//vector<antlrcpp::Any>
    auto op_list = ctx->comp_op();//vector
    if (arithArray.size() == 1){
        return visitArith_expr(arithArray[0]);
    }
    //注意dfs的写法
    New_Any l = Query(visitArith_expr(arithArray[0]) );

    for (int i = 1;i < arithArray.size();++i){ //从第二个开始
        string op = op_list[i - 1]->getText();
        New_Any r = Query(visitArith_expr(arithArray[i]) );
        //连续的判断是用and连接的
        if (op == ">" && l <= r ||
            op == "<" && l >= r ||
            op == ">=" && l < r ||
            op == "<=" && l > r ||
            op == "==" && l != r ||
            op == "!=" && l == r)
            return New_Any(false);
        l = r;
    }

    return New_Any(true);
}

//todo: 加法和减法
antlrcpp::Any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
    auto termArray = ctx->term();//vector
    if (termArray.size() == 1)
        return visitTerm(termArray[0]);//不需要.as<New_Any>()

    auto opArray = ctx->addorsub_op();
    New_Any ret = Query(visitTerm(termArray[0]) );
    for (int i = 1; i < termArray.size(); ++i) {
        std::string tmpOp = opArray[i - 1]->getText();

        New_Any temp = Query(visitTerm(termArray[i]) );

        if (tmpOp == "+") ret += temp;
        else if (tmpOp == "-") ret -= temp;
//            else throw Exception("", UNIMPLEMENTED);
    }
    return ret;
}

//todo: 乘法，整除，浮点除，取模
antlrcpp::Any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
    auto factorArray = ctx->factor();
    if (factorArray.size() == 1) return visitFactor(factorArray[0]);

    New_Any ans;
    auto opArray = ctx->muldivmod_op();
    New_Any ret = Query(visitFactor(factorArray[0]) );
    for (int i = 1; i < factorArray.size(); ++i) {
        std::string tmpOp = opArray[i - 1]->getText();

        New_Any temp = Query(visitFactor(factorArray[i]) );

        if (tmpOp == "*") ret *= temp;
        else if (tmpOp == "//" || tmpOp == "/") ret /= temp;
        else if (tmpOp == "%") ret %= temp;
//            else throw Exception("", UNIMPLEMENTED);
    }
    return ret;
}

antlrcpp::Any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
//    if (ctx->atom_expr()) return visitAtom_expr(ctx->atom_expr());
    return visitChildren(ctx);
}

//返回的是New_Any
antlrcpp::Any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
    if (!ctx->trailer()) return visitAtom(ctx->atom());
    auto functionName = ctx->atom()->getText();
    //todo:需要修改
    //返回的是vector<pair<string,New_Any>>
    auto argsArray = visitTrailer(ctx->trailer()).as<std::vector<New_Any> >();

    if (functionName == "print") {
        std::cout << argsArray[0];
        for (int i = 1; i < argsArray.size(); ++i)
            std::cout << " " << argsArray[i];
        std::cout << '\n';
        return New_Any();
    } else if (functionName == "int") {
        return New_Any(argsArray[0]);
    } else if (functionName == "float") {

    } else if (functionName == "bool") {

    }
    else {

    }
}

antlrcpp::Any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
    if (ctx->arglist()) return visitArglist(ctx->arglist());//递归
    return std::vector<New_Any>();
}

antlrcpp::Any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
    if (ctx->NUMBER()) {//是数字
        string s = ctx->NUMBER()->getText();
        //Attention!读入的优先级是double>int>string
        if (s.find('.') == s.npos) return New_Any(int2048(s));//找不到.说明是int
        else return New_Any(stod(s));//是double
    }
    else if (ctx->NAME()) {//是字符串(变量的名称)
        auto result = scope.varQuery(ctx->NAME()->getText());
        if (result.first) {//该变量存在
            return result.second;
        }
        else return New_Any();
    }
    else if (ctx->NONE()) return New_Any();
    else if (ctx->TRUE()) return New_Any(true);
    else if (ctx->FALSE()) return New_Any(false);
    else if (ctx->test()) return visitTest(ctx->test());//继续dfs
    else {//是字符串(可能有多个,要+连接)
        string ans;
        auto str_list = ctx->STRING();
        for (auto i:str_list){
            ans += i->getText();

        }
        return New_Any(ans);
    }
//        throw Exception("", UNIMPLEMENTED);
}

//todo:
antlrcpp::Any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
    auto argumentArray = ctx->argument();
    std::vector<New_Any> retArray;
    for (auto ctx: argumentArray) {//建议使用这种遍历,可以提高效率

        retArray.push_back(visitTest(ctx->test()[0]));
    }
    return retArray;
}

antlrcpp::Any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
    return visitChildren(ctx);
}

