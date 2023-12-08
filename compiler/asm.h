#include <optional>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <unordered_map>
#include <algorithm>
#include "ansi.h"
#include "token.h"
#include "parser.h"


class Generator {
public:
    inline explicit Generator(NodeProg prog)
        : m_prog(std::move(prog))
    {}

    void gen_expr(const NodeExpr& expr) {
        struct ExprVisitor {
            Generator* gen;

            void operator()(const NodeExprIntLit& expr_int_lit) {
                gen->m_output << "    mov rax, " << expr_int_lit.int_lit.value.value() << "\n";
                gen->push("rax");

            }
            void operator()(const NodeExprIdent& expr_ident) {
                // Check if variable exists
                auto it = gen->m_vars.find(expr_ident.ident.value.value());
                if (it == gen->m_vars.end()) {
                    std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Undeclared identifier: '" << expr_ident.ident.value.value()  << "'" << std::endl;
                    std::cerr << "compilation terminated." << std::endl;
                    exit(EXIT_FAILURE);
                }
                const auto& var = gen->m_vars.at(expr_ident.ident.value.value());
                std::stringstream offset;
                offset << "QWORD [rsp + " << (gen->m_stack_size - var.stack_loc - 1) * 8 << "]\n";
                gen->push(offset.str());
            }
        };
        ExprVisitor visitor{.gen = this};
        std::visit(visitor, expr.var);
    }

    void gen_stmt(const NodeStmt& stmt) {
        struct StmtVisitor {
            Generator* gen;

            void operator()(const NodeStmtExit& stmt_exit)
            {
                gen->gen_expr(stmt_exit.expr);
                gen->m_output << "    mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "    syscall\n";
            }
            void operator()(const NodeStmtInt& stmt_int)
            {
                auto it = gen->m_vars.find(stmt_int.ident.value.value());
                if (it != gen->m_vars.end()) {
                    std::cerr << ANSI_BOLD_WHITE << "ent: " << ANSI_BOLD_RED << "fatal error: " << ANSI_RESET_COLOR << "Identifier already in use: '" << stmt_int.ident.value.value()  << "'" << std::endl;
                    std::cerr << "compilation terminated." << std::endl;
                    exit(EXIT_FAILURE);
                }
                gen->m_vars.insert({stmt_int.ident.value.value(), Var {.stack_loc = gen->m_stack_size}});
                gen->gen_expr(stmt_int.expr);
            }
        };

        StmtVisitor visitor{.gen = this};
        std::visit(visitor, stmt.var);
    }

    [[nodiscard]] std::string gen_prog()
    {
        
        m_output << "global _start\n_start:\n";
        
        // Code generation
        for (const NodeStmt& stmt : m_prog.stmts) {
            gen_stmt(stmt);
        }

        m_output << "    mov rax, 60\n";
        m_output << "    mov rdi, 0\n";
        m_output << "    syscall\n";
        return m_output.str();
    }
private:

    void push(const std::string& reg) {
        m_output << "    push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const std::string& reg) {
        m_output << "    pop " << reg << "\n";
        m_stack_size--;
    }

    struct Var {
        size_t stack_loc;
    };

    const NodeProg m_prog;
    size_t m_stack_size = 0;
    std::stringstream m_output;
    std::unordered_map<std::string, Var> m_vars {};

};