using System;
using System.Management;

namespace WMI
{

    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                // Подписка на WMI-событие для отслеживания входа нового пользователя
                ManagementEventWatcher watcher = new ManagementEventWatcher(
                    //new WqlEventQuery("SELECT * FROM Win32_LogonSession ")
                    new WqlEventQuery("__InstanceCreationEvent", new TimeSpan(0, 0, 1), "TargetInstance ISA \"Win32_LogonSession\"")
                );
                watcher.EventArrived += new EventArrivedEventHandler(HandleEvent);

                // Начало отслеживания
                watcher.Start();

                Console.WriteLine("Ожидание входа нового пользователя... Нажмите Enter для выхода.");
                Console.ReadLine();

                // Остановка отслеживания
                watcher.Stop();
            }
            catch (Exception e)
            {
                Console.WriteLine("Произошла ошибка: " + e.Message);
            }
        }

        //private readonly static WqlEventQuery qLgi = new WqlEventQuery("__InstanceCreationEvent", new TimeSpan(0, 0, 1), "TargetInstance ISA \"Win32_LogonSession\"");

        static void HandleEvent(object sender, EventArrivedEventArgs e)
        {
            // Получение информации о событии
            ManagementBaseObject targetInstance = (ManagementBaseObject)e.NewEvent["TargetInstance"];

            foreach (PropertyData property in targetInstance.Properties)
            {
                Console.WriteLine($"{property.Name}: {property.Value}");
            }

            // Здесь можно добавить дополнительную логику для извлечения имени пользователя
            // Но для этого потребуется дополнительный запрос к системе, так как Win32_LogonSession
            // не содержит прямой ссылки на имя пользователя
        }
    }
}