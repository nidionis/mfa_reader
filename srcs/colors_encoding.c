//
// Created by nidionis on 10/06/25.
//

// 'Encodes' four individual bytes into an int.
int get_rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

////https://harm-smits.github.io/42docs/libs/minilibx/colors.html
//int	create_trgb(int t, int r, int g, int b)
//{
//    return (t << 24 | r << 16 | g << 8 | b);
//}
