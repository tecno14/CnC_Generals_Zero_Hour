namespace Models;

public class Project
{
    public string Name { get; set; }

    public string Path { get; set; }

    public string Guid { get; set; }

    public List<string> References { get; set; } = [];

    public List<string> ReferencedBy { get; set; } = [];
}
