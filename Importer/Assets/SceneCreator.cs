using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Importer))]
public class SceneCreator : MonoBehaviour
{
    public Material defaultMaterial;

    // Singleton.
    private static SceneCreator instance;
    public static SceneCreator Instance
    {
        get
        {
            if (instance == null)
            {
                instance = FindObjectOfType<SceneCreator>();
            }

            return instance;
        }
    }

    public static Material GetNewDefaultMaterial()
    {
        Material newMaterial = Object.Instantiate(Instance.defaultMaterial);
        return newMaterial;
    }

    private void Start()
    {
        ImportDataTypes.Scene scene = this.GetComponent<Importer>().scene;

        for (int i = 0; i < scene.header.transformCount; i++)
        {
            CreateTransform(scene.body.transforms[i]);
        }

        for (int i = 0; i < scene.header.meshCount; i++)
        {
            ImportDataTypes.Mesh mesh = scene.body.meshes[i];
            Transform obTransform = GameObject.Find(mesh.header.transformName.text).transform;
            CreateMesh(obTransform, mesh.body.vertices, null, defaultMaterial);
        }
    }

    public UnityEngine.Transform CreateTransform(ImportDataTypes.Transform transform)
    {
        GameObject ob = new GameObject();
        ob.name = transform.name.text;
        ob.transform.position = transform.position.Create();
        ob.transform.localEulerAngles = transform.rotation.Create();
        ob.transform.localScale = transform.scale.Create();
        return ob.transform;
    }

    public void CreateMesh(ImportDataTypes.Mesh mesh, ImportDataTypes.Transform transform)
    {
        GameObject meshOB = transform.gameObject;
        Mesh mesh = new Mesh();

        MeshFilter meshFilter = meshOB.AddComponent<MeshFilter>();
        meshFilter.mesh = mesh;

        MeshRenderer meshRenderer = meshOB.AddComponent<MeshRenderer>();

        material = material != null ? material : defaultMaterial;
        meshRenderer.material = material;

        mesh.vertices = vertices;
        mesh.triangles = triangles;
        mesh.RecalculateNormals();
    }

    private void TestScene()
    {
        Datatypes.Transform tra = new Datatypes.Transform();
        tra.position = Vector3.zero;
        tra.rotation = Vector3.zero;
        tra.scale = Vector3.one;
        tra.name = "myTransform";

        Datatypes.Light light = new Datatypes.Light();
        light.intensity = 10;
        light.range = 25;
        light.transform = tra;
        light.Create();

        // Basic Quad.
        List<Vector3> vertices = new List<Vector3>();

        vertices.Add(new Vector3(0, 0, 0));
        vertices.Add(new Vector3(1, 0, 0));
        vertices.Add(new Vector3(0, 1, 0));
        vertices.Add(new Vector3(1, 1, 0));

        List<int> tri = new List<int>();

        //  Lower left triangle.
        tri.Add(0);
        tri.Add(2);
        tri.Add(1);

        //  Upper right triangle.   
        tri.Add(2);
        tri.Add(3);
        tri.Add(1);
        // Basic Quad.

        //*************
        // Basic Quad morph position TEST.
        List<Vector3> verticesMorph = new List<Vector3>();

        verticesMorph.Add(new Vector3(0, 0, 0));
        verticesMorph.Add(new Vector3(2, 0, 0));
        verticesMorph.Add(new Vector3(0, 1, 0));
        verticesMorph.Add(new Vector3(2, 1, 0));
        Datatypes.MorphTarget morphTarget1 = new Datatypes.MorphTarget(verticesMorph.ToArray());
        Datatypes.MorphTarget morphTarget2 = new Datatypes.MorphTarget(vertices.ToArray());

        List<Datatypes.MorphTarget> morphTargets = new List<Datatypes.MorphTarget>();
        morphTargets.Add(morphTarget1);
        morphTargets.Add(morphTarget2);

        // TESTING MORPH
        Datatypes.Mesh mesh = new Datatypes.Mesh(tra, vertices.ToArray(), tri.ToArray(), morphTargets);
        mesh.Create();
        //*************

        //CreateMesh(vertices.ToArray(), tri.ToArray(), defaultMaterial);
    }
}
