#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

int main(int argc, char **argv, char **env)
{
	if (argc == 1)
	{
		printf("Использование: %s [env_var] [env_var1]. \n", argv[0]);
		printf("Выводит значения указанных переменных среды.\n");
		printf("Названия переменных НЕ ырегистрозависимы!!\n");
		return 0;
	}

	for (int i = 1; i < argc; i++)
	{
		//имя переменной
		char *var_name = argv[i]; 
		//значение переменной
		char *value = NULL;
		//копия имени переменной для вывода
		char *real_name = NULL;
		//env_ptr - указатель на переменные окружения
		for (char **env_ptr = env; *env_ptr != NULL; env_ptr++) // перебор всех строк окружения
		{
			char *env_var = *env_ptr; //строка в виде name=value(переменные окружения)
			char *d = strchr(env_var, '='); // разделитель

			if (d != NULL) //если найден разделитель
			{
				size_t name_len = d - env_var; //длина имени переменной(разница указателей) до знака =
				if (strlen(var_name) == name_len && // совпадает ли длина строки
					strncasecmp(env_var, var_name, name_len) == 0) //не учтение регистра и совпадение имени переменной с искомой
				{
					real_name = strndup(env_var, name_len); //копирование имени из окружения
					value = d + 1; //берем значения после =
					break;
				}
			}
		}

		printf("%s\n", real_name ? real_name : var_name); //вывод найденной переменной или исходное имя
		printf("%s\n", value ? value : "");	//вывод значение, либо же пустой строки	  
		printf("---\n");								 

		if (real_name)
		{
			free(real_name); //освобождение памяти
		}
	}

	return 0;
}
