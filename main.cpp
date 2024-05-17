#include <gtk/gtk.h>
#include "Soduku.h"
#include "Interface.h"
#include <iostream>
#include <string>
#define N 9


GtkWidget *label[N][N];
PuzzleSoduku puzzle;

///******************************       Callback evenements     ******************************///
// Fonction de callback appel�e lors de la fermeture de la fen�tre
static void on_window_closed(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static void checkClicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    char text[17];
    int check = 0 ;

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
             const gchar *textValue = gtk_label_get_text(GTK_LABEL(label[i][j]));
             int value = atoi(textValue);
             g_print("Integer value: %d\n", value);
             if(value != puzzle.solution[i][j])
             {
                 check = 0;
                 break;
             }
             else
                check = 1;
        }
    }

    if(check==1)
        strcpy(text,"Bien   joué    !");
    else
        strcpy(text,"Mauvaise   réponse ");

    // Création d'une boîte de dialogue
    dialog = gtk_message_dialog_new(GTK_WINDOW(data),
                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                     GTK_MESSAGE_INFO,
                                     GTK_BUTTONS_OK,
                                     text);


    GtkStyleContext *context = gtk_widget_get_style_context(dialog);
    gtk_style_context_add_class(context, "dialog");
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);
    // Affichage de la boîte de dialogue
    gtk_dialog_run(GTK_DIALOG(dialog));

    // Fermeture de la boîte de dialogue
    gtk_widget_destroy(dialog);
}


static void solveClicked(GtkButton *button, gpointer user_data)
{

    for(int i=0;i<N;i++)
    {
        for(int j=0; j<N; j++)
        {
            gchar label_text[2];
            if(puzzle.solution[i][j] != puzzle.board[i][j])
            {
                GtkStyleContext *context = gtk_widget_get_style_context(label[i][j]);
                gtk_style_context_add_class(context, "solved");
                sprintf(label_text, "%d",puzzle.solution[i][j]);
                gtk_label_set_text(GTK_LABEL(label[i][j]),  label_text);
            }
        }
    }
}

static void clearClicked(GtkButton *button, gpointer user_data)
{
    gchar label_text[2];
    for(int i=0;i<N;i++)
    {
        for(int j=0; j<N; j++)
        {
            if(puzzle.board[i][j]==0)
                sprintf(label_text, "");
            else
                sprintf(label_text, "%d",puzzle.board[i][j] );
            GtkStyleContext *context = gtk_widget_get_style_context(label[i][j]);
            gtk_style_context_add_class(context, "small-box");
            gtk_label_set_text(GTK_LABEL(label[i][j]),  label_text);
        }
    }
}

static void genClicked(GtkButton *button, gpointer user_data)
{
    puzzle.initialiserGame();
    puzzle.afficher();
    gchar label_text[2];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if(puzzle.board[i][j]==0)
                sprintf(label_text, "");
            else
                sprintf(label_text, "%d",puzzle.board[i][j] );
            GtkStyleContext *context = gtk_widget_get_style_context(label[i][j]);
            gtk_style_context_add_class(context, "small-box");
            gtk_label_set_text(GTK_LABEL(label[i][j]),  label_text);
        }
    }

}

///*******************          Class Button        *******************///
class Button
{
public:
    Button(const char* label,GCallback func,gint widtht, gint height,int type );
    GtkWidget *getButton();

private:
    GtkWidget *button;
};


Button::Button(const char* label,GCallback func, gint width, gint height,int type)
{
    button = gtk_button_new_with_label(label);
    gtk_widget_set_size_request(GTK_WIDGET(button), width,  height);
    g_signal_connect(button, "clicked", func, NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(button);
    switch(type)
    {
        case 1:
            gtk_style_context_add_class(context, "button-num");
            break;
        default:
            gtk_style_context_add_class(context, "button");
            break;
    }
}

GtkWidget *Button::getButton()
{
    return button;
}

///*******************          Class Interface        *******************///
class Interface
{
public:
    GtkWidget *mainContainer;
    Interface();
    void createBoard();
private:
    GtkWidget* board;
    GtkWidget* box[N];
    GtkEventBox *event_carre[N][N];
};

/**
*   Cette fonction permet de construire le puzzle soduku
*/
void Interface::createBoard()
{
    int i,j,k,l, numBox=0;

    /// creation du board qui va contenir le puzzle soduku
    this->board = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(this->board), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(this->board), TRUE);
    gtk_widget_set_size_request(GTK_WIDGET(board), 500, 500);
    /// CSS du board
    GtkStyleContext *context = gtk_widget_get_style_context(this->board);
    gtk_style_context_add_class(context, "box");

    ///construction des petites carreaux du puzzle
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            gchar *name = g_strdup_printf("grid_%d_%d", i, j);
            gchar label_text[2];
            if(puzzle.board[i][j]==0)
                sprintf(label_text, "");
            else
                sprintf(label_text, "%d",puzzle.board[i][j] );
            label[i][j] = gtk_label_new(label_text);
            ///CSS label
            GtkStyleContext *context = gtk_widget_get_style_context(label[i][j]);
            gtk_style_context_add_class(context, "small-box");
            /// evenement pour capturer le click sur le petit carreau
            event_carre[i][j] = (GtkEventBox*) gtk_event_box_new();
            gtk_widget_set_name(GTK_WIDGET(event_carre[i][j]), name);
            g_signal_connect(G_OBJECT(event_carre[i][j]), "button-press-event", G_CALLBACK(carreClicked), NULL);
            /// ajout du label qui va contenir le numéro
            gtk_container_add(GTK_CONTAINER(event_carre[i][j]), GTK_WIDGET(label[i][j]));
            g_free(name);
        }
    }

    ///création des box (1-9) du puzzle soduku
    for(i=0; i<3; i++)
    {
        for(j=0; j<3; j++)
        {
            box[numBox] = gtk_grid_new();
            gtk_grid_set_row_homogeneous(GTK_GRID(box[numBox]), TRUE);
            gtk_grid_set_column_homogeneous(GTK_GRID(box[numBox]), TRUE);
            gtk_widget_set_size_request(GTK_WIDGET(box[numBox]), 50, 50);
            GtkStyleContext *context = gtk_widget_get_style_context(box[numBox]);
            gtk_style_context_add_class(context, "box");
            /// ajout du box à la grid principal
            gtk_grid_attach(GTK_GRID(board), GTK_WIDGET(box[numBox]), j, i, 1, 1);
            ///ajout des petites carreaux au box selon leur positions
            for(k=0; k<3; k++)
            {
                for(l=0; l<3; l++)
                {
                    switch (numBox)
                    {
                        case 0:
                            gtk_grid_attach(GTK_GRID(box[0]), GTK_WIDGET(event_carre[k][l]), l, k, 1, 1);
                            break;
                        case 1:
                            gtk_grid_attach(GTK_GRID(box[1]), GTK_WIDGET(event_carre[k][l+3]), l, k, 1, 1);
                            break;
                        case 2:
                            gtk_grid_attach(GTK_GRID(box[2]), GTK_WIDGET(event_carre[k][l+6]), l, k, 1, 1);
                            break;
                        case 3:
                            gtk_grid_attach(GTK_GRID(box[3]), GTK_WIDGET(event_carre[k+3][l]), l, k, 1, 1);
                            break;
                        case 4:
                            gtk_grid_attach(GTK_GRID(box[4]), GTK_WIDGET(event_carre[k+3][l+3]), l, k, 1, 1);
                            break;
                        case 5:
                            gtk_grid_attach(GTK_GRID(box[5]), GTK_WIDGET(event_carre[k+3][l+6]), l, k, 1, 1);
                            break;
                        case 6:
                            gtk_grid_attach(GTK_GRID(box[6]), GTK_WIDGET(event_carre[k+6][l]), l, k, 1, 1);
                            break;
                        case 7:
                            gtk_grid_attach(GTK_GRID(box[7]), GTK_WIDGET(event_carre[k+6][l+3]), l, k, 1, 1);
                            break;
                        case 8:
                            gtk_grid_attach(GTK_GRID(box[8]), GTK_WIDGET(event_carre[k+6][l+6]), l, k, 1, 1);
                            break;
                    }
                }
            }
            ///passer au box suivant
            numBox++;
        }
    }
}

/**
*   Cette fonction permet de construire l'interface qui contient le puzzle et les bouttons d'interraction
*/
Interface::Interface()
{
    this->createBoard();

    ///création des contenaires de type GtkBox
    this->mainContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);     ///box qui va contenir les autres box
    GtkWidget *boxNumbers = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); ///box qui va contenir les boutons numéros
    GtkWidget *boxButtons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); ///box qui va contenir les autres boutons

    ///création des bouton numéros
    for (int i = 1; i <= 9; ++i)
    {
        gchar *button_name = g_strdup_printf("%d", i);
        Button numButt(button_name,G_CALLBACK(buttonClicked),10,10,1);
        g_free(button_name);
        ///ajout au box des boutons numéros
        gtk_box_pack_start(GTK_BOX(boxNumbers), numButt.getButton(), FALSE, TRUE, 0);
    }

    Button solveButt("Solve",G_CALLBACK(solveClicked),40,10,0);     ///bouton qui permet de résoudre le puzzle
    Button clearButt("Clear",G_CALLBACK(clearClicked),30,10,0);     ///bouton qui permet de vider le puzzle de la saisie d'utilisateur
    Button checkButt("Check",G_CALLBACK(checkClicked),30,10,0);     ///bouton qui permet de vérifier la réponse d'utilisateur

    ///ajout au box des autres boutons
    gtk_box_pack_start(GTK_BOX(boxButtons),solveButt.getButton(), FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(boxButtons), gtk_label_new(""), TRUE, TRUE, 0);  // Space
    gtk_box_pack_start(GTK_BOX(boxButtons),clearButt.getButton(), FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(boxButtons), gtk_label_new(""), TRUE, TRUE, 0);  // Space
    gtk_box_pack_start(GTK_BOX(boxButtons),checkButt.getButton(), FALSE, TRUE, 0);

    ///ajout au box principal
    gtk_box_pack_start(GTK_BOX(mainContainer),this->board, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(mainContainer),boxNumbers, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(mainContainer),boxButtons, FALSE, TRUE, 0);
}

int main(int argc, char *argv[]) {
    // Initialisation de GTK
    gtk_init(&argc, &argv);

    ///uploading the css style
    load_css();

   /* int grille[N][N]= {{3, 0, 6,    5, 0, 8,    4, 0, 0},
                       {5, 2, 0,     0, 0, 0,    0, 0, 0},
                       {0, 8, 7,     0, 0, 0,    0, 3, 1},
                       {0, 0, 3,     0, 1, 0,    0, 8, 0},
                       {9, 0, 0,     8, 6, 3,    0, 0, 5},
                       {0, 5, 0,     0, 9, 0,    6, 0, 0},
                       {1, 3, 0,     0, 0, 0,    2, 5, 0},
                       {0, 0, 0,     0, 0, 0,    0, 7, 4},
                       {0, 0, 5,     2, 0, 6,    3, 0, 0}};
    ///construction du puzzle soduku
    PuzzleSoduku mypuzzle(grille);
    puzzle = mypuzzle;*/
    ///initialiser le soduku
    puzzle.initialiserGame();

    ///construction de l'interface
    Interface interface;

    /// creation de la fenetre d'affichage
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Soduku");
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    /// Connecter la fonction de callback d l'evenement de fermeture de la fenetre
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(on_window_closed), NULL);

    /// Creation du a menu bar
    GtkWidget *menu_bar = gtk_menu_bar_new();

    GtkWidget *file_menu_item = gtk_menu_item_new_with_label("Fichier");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_menu_item);

    GtkWidget *file_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);

    GtkWidget *open_menu_item = gtk_menu_item_new_with_label("New Game");
    g_signal_connect(open_menu_item, "activate", G_CALLBACK(genClicked), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_menu_item);

    GtkWidget *exit_menu_item = gtk_menu_item_new_with_label("Exit");
    g_signal_connect(exit_menu_item, "activate", G_CALLBACK(gtk_main_quit), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit_menu_item);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); ///box qui va contenir le menu et maincontainer
    gtk_box_pack_start(GTK_BOX(box),menu_bar, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box),interface.mainContainer, FALSE, TRUE, 0);


    ///ajout de l'interface soduku à la fenetre
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);
    // D�marrer la boucle principale GTK
    gtk_main();

    return 0;
}
