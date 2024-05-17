//
// Created by Achra on 1/19/2024.
//

#ifndef SODUKU_INTERFACE_H
#define SODUKU_INTERFACE_H

std::string clickedButton="";


static void carreClicked(GtkWidget *event_box, GdkEventButton *event, gpointer user_data) {
    GtkWidget *label = gtk_bin_get_child(GTK_BIN(event_box));

    const int length = clickedButton.length();

    char* chaine = new char[length + 1];

    strcpy(chaine, clickedButton.c_str());

    gtk_label_set_text(GTK_LABEL(label),  chaine);
    delete[]  chaine;
}

static void buttonClicked(GtkButton *button, gpointer user_data)
{
     const gchar *button_label = gtk_button_get_label(button);
     std::string str(button_label, strlen(button_label));
     clickedButton = str;
     std::cout << "Converted string value: " << str << std::endl;

    g_print("Button %s clicked!\n", button_label);
}

static void load_css ( void )
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    const gchar *css_style_file = "style.css";
    GFile *css_fp = g_file_new_for_path ( css_style_file );
    GError *error = 0;

    provider = gtk_css_provider_new ();
    display = gdk_display_get_default ();
    screen = gdk_display_get_default_screen ( display );
    gtk_style_context_add_provider_for_screen ( screen, GTK_STYLE_PROVIDER ( provider ), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_css_provider_load_from_file ( provider, css_fp, &error );
    g_object_unref ( provider );
}

int getIndexBox(int row,int col)
{
    if(row<3)
    {
        if(col<3) return 0;
        if(col<6) return 1;
        if(col<9) return 2;
    }
    if(row<6)
    {
        if(col<3) return 3;
        if(col<6) return 4;
        if(col<9) return 5;
    }
    else
    {
        if(col<3) return 6;
        if(col<6) return 7;
        if(col<9) return 8;
    }
    return -1;
}

GtkWidget* Board()
{
    GtkWidget* board;
    GtkWidget* box[N];
    GtkWidget* carre[N][N];
    GtkEventBox *event_carre[N][N];
    int boxi, boxj;
    int i,j,k,l, numBox=0;

    board = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(board), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(board), TRUE);
    gtk_widget_set_size_request(GTK_WIDGET(board), 500, 500);



    // Apply CSS styles directly in code
    GtkStyleContext *context = gtk_widget_get_style_context(board);
    gtk_style_context_add_class(context, "box");




    GtkWidget *label[N][N];

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            gchar *name = g_strdup_printf("grid_%d_%d", i, j);
            gchar label_text[4];
            sprintf(label_text, "%d,%d", i, j);
            label[i][j] = gtk_label_new(label_text);
            // Initialize event box to capture signals associated with carreGrid
            event_carre[i][j] = (GtkEventBox*) gtk_event_box_new();
            gtk_widget_set_name(GTK_WIDGET(event_carre[i][j]), name);
            g_signal_connect(G_OBJECT(event_carre[i][j]), "button-press-event", G_CALLBACK(carreClicked), NULL);

            // Add carreGrid to event_box
            gtk_container_add(GTK_CONTAINER(event_carre[i][j]), GTK_WIDGET(label[i][j]));

            g_free(name);
        }
    }

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
            // Add event_box to the main Grid
            gtk_grid_attach(GTK_GRID(board), GTK_WIDGET(box[numBox]), j, i, 1, 1);

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
            numBox++;
        }
    }


    return board;
}


#endif //SODUKU_INTERFACE_H
