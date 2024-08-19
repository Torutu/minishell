Tokenization is the process of breaking down a string of text into smaller components called tokens.
in the project Minishell the tokenization will take each command line input into blocks of valuable information that can be used for parsing and execution.

so far the tokenization stage feels important as it is the first part of any command and string goes through the Minishell it transforms a string of text into a structured format that can be easily manipulated used and accessed by everyone in this project.
it ensures that each command, argument and operator is identified correctly

## Process

- Reading input: reads the command line input as a single string.
- Identifying tokens: the input string is analyzed to find arguments, operators and commands that are separated by delimiters.
- Generating token list: The identified tokens are stored in a linked list of structures in the data structure.

![Pasted image 20240711234609](https://github.com/user-attachments/assets/ad996c31-6f8b-4afb-99ed-fd30014f353f)

<!-- uncomment to use it in obsidian ![[Tokenization visualized.canvas]] -->

## The token structure
so far the struct has 4 variables:
- **Value**   : will hold the string of the tokenized command line.
- **type**    : enum that defines what type of token(e.g, command, builtin...) <- it returns number that's already defined in enum.
- **id**      : an integer holds the index or position of the token.
- **next**    : this will point to the next token, the last token will point to null just like any linked list
## How to use the tokens
```
void    print_tokens(t_data *data) // the token struct is in the data struct
{
        t_token *token = data->token; //create a local copy of the token_structure
        while (token != NULL)
        {
                printf("token value:[%s]\n", token->value);
				printf("token type:[%i]\n", token->type);
                printf("id:[%i]\n", token->id);
                token = token->next; //move to the next struct or NULL if last one
        }
}
```

in that function we used the token's data to be printed for us in the terminal which will create a very similar output to the previous example

![Pasted image 20240712001525](https://github.com/user-attachments/assets/eaf8af56-ba68-439a-8050-c8fb053de34e)

you can have your token struct separately but it is better to have all your variables and structs be in one large data struct so you only add as little parameters as possible in your function and just create a local copy of the variable you want to use.
(there could be some issues with that but so far I haven't found any)


## pipes, redirections and syntax

my plan for now is to make each command its own pipe group here's an example

```bash
ls -la | wc -w
```

![[pipes.canvas]]